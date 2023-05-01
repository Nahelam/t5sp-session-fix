#include <stdinc.hpp>
#include <utils/hook.hpp>

#include "loader/component_loader.hpp"
#include "component/scheduler.hpp"
#include "game/game.hpp"

#define SESSION_MODIFY_MAX_FAILURES 3
#define SESSION_CREATE_MAX_FAILURES 3

#define RECONNECT_MAX_WATCH 3

namespace session_fix
{
    namespace
    {
        utils::hook::detour Session_StartHost_hook;
        utils::hook::detour Session_StartHostComplete_hook;
        utils::hook::detour Session_Modify_hook;
        utils::hook::detour Session_ModifyComplete_hook;
        utils::hook::detour Session_UpdateNetCode_hook;
        utils::hook::detour Cbuf_AddText_hook;

        // Keep track of the moment we entered in the session recreate state
        std::chrono::steady_clock::time_point session_recreate_time;

        bool session_recreate = false;
        bool session_recreate_in_progress = false;
        bool reconnect = false;

        void session_fix_print(const char* format, ...)
        {
            char msg[0x100] = {};

            va_list args;
            va_start(args, format);

            game::_vsnprintf(msg, sizeof(msg), format, args);
            game::Com_Printf(game::CON_CHANNEL_SYSTEM, "[SESSION FIX] %s\n", msg);
        }

        void print_bdLobbyErrorCode(game::overlappedTask* task)
        {
            game::bdLobbyErrorCode errorCode;
            const void* bdRemoteTask = task->overlappedIO;

            // __thiscall
            __asm
            {
                mov ecx, bdRemoteTask
                call game::bdRemoteTask_getErrorCode
                mov errorCode, eax
            }

            if (errorCode == game::BD_NO_ERROR)
            {
                return;
            }

            char errorStr[0x40] = {};

            game::dwLobbyErrorCodeToString(errorCode, errorStr, sizeof(errorStr));
            session_fix_print("Session update error %i - '%s'", errorCode, errorStr);
        }

        void dwCloseRemoteTask_stub(game::overlappedTask* task)
        {
            print_bdLobbyErrorCode(task);
            utils::hook::invoke<void>(game::dwCloseRemoteTask, task);
        }

        void session_delete()
        {
            // Disable non-blocking mode to ensure the session deletion
            (*game::session_nonblocking)->current.enabled = false;
            game::Session_DeleteSession(game::g_serverSession);
            (*game::session_nonblocking)->current.enabled = true;
        }

        void session_create()
        {
            game::Session_StartHost(game::g_serverSession, 0xBAD5E55, 4, 4);
        }

        void session_set_recreate()
        {
            session_recreate = true;
            session_recreate_time = std::chrono::high_resolution_clock::now();
        }

        void force_reconnect()
        {
            *game::dwControllerData_logOnAttempts = 0;
            *game::dwControllerData_forceLogOn = true;
        }

        game::taskCompleteResults Session_StartHostComplete_stub(const int slot)
        {
            static unsigned int session_create_error_count = 0;

            game::taskCompleteResults result;

            result = Session_StartHostComplete_hook.invoke<game::taskCompleteResults>(slot);

            if (session_recreate && result != game::TASK_NOTCOMPLETE)
            {
                if (result == game::TASK_COMPLETE)
                {
                    session_fix_print("Session recreated successfully");
                    game::Party_ReAddAllPlayers(game::g_lobbyData);
                    session_recreate = false;
                }
                else if (result == game::TASK_ERROR)
                {
                    session_fix_print("Failed to recreate session");
                    session_create_error_count++;

                    if (session_create_error_count == SESSION_CREATE_MAX_FAILURES)
                    {
                        reconnect = true;
                        session_create_error_count = 0;
                        session_fix_print("Reached session create failures limit, will attempt a whole reconnect...");
                    }
                    else
                    {
                        session_fix_print("Will try to recreate session again soon...");
                    }
                }

                session_recreate_in_progress = false;
            }
            else if (!session_recreate && result == game::TASK_ERROR)
            {
                // The server just started and failed to create a session
                session_set_recreate();
            }

            return result;
        }

        void Session_StartHost_stub(game::SessionData_s* session, int sessionFlags, int numPrivateSlots, int numPublicSlots)
        {
            if (session_recreate || reconnect)
            {
                if (sessionFlags != 0xBAD5E55)
                {
                    return;
                }
                else
                {
                    sessionFlags = 1;
                }
            }

            Session_StartHost_hook.invoke<void>(session, sessionFlags, numPrivateSlots, numPublicSlots);
        }

        void Session_Modify_stub(const int localControllerIndex, game::SessionData_s* session, const int flags, const int publicSlots, const int privateSlots)
        {
            if (!session_recreate)
            {
                Session_Modify_hook.invoke<void>(localControllerIndex, session, flags, publicSlots, privateSlots);
            }
        }

        void Session_UpdateNetCode_stub(const int netCodeVersion)
        {
            if (!session_recreate)
            {
                Session_UpdateNetCode_hook.invoke<void>(netCodeVersion);
            }
        }

        void Cbuf_AddText_stub(int localClientNum, const char* text)
        {
            static bool first_map_rotate_executed = false;

            if (reinterpret_cast<int>(text) == 0x19E498)
            {
                if (!first_map_rotate_executed)
                {
                    first_map_rotate_executed = true;
                }
                else
                {
                    return;
                }
            }

            Cbuf_AddText_hook.invoke<void>(localClientNum, text);
        }

        game::taskCompleteResults Session_ModifyComplete(/* const int slot (in eax) */)
        {
            int slot;
            __asm
            {
                mov slot, eax
            }

            game::taskCompleteResults result;
            game::overlappedTask* sessionModifyOverlappedIO;

            sessionModifyOverlappedIO = &((*game::overlappedTasks_2)[slot]);

            if (session_recreate)
            {
                session_fix_print("Clearing session modify task...");
                game::TaskManager_ClearTask(sessionModifyOverlappedIO);
                return game::TASK_COMPLETE;
            }

            result = game::dwUpdateSessionComplete(sessionModifyOverlappedIO);

            if (result == game::TASK_ERROR)
            {
                game::Com_Printf(game::CON_CHANNEL_SYSTEM, "EXE_ERROR_MODIFYING_SESSION\n");
            }
            else if (result == game::TASK_COMPLETE)
            {
                game::Com_Printf(game::CON_CHANNEL_SYSTEM, "Session modify complete\n");

                if ((*game::onlinegame)->current.enabled && game::Party_AreWeHost(game::g_lobbyData))
                {
                    game::PartyHost_GamestateChanged(game::g_lobbyData);
                }
            }

            if (result != game::TASK_NOTCOMPLETE)
            {
                game::TaskManager_ClearTask(sessionModifyOverlappedIO);
            }

            static unsigned int session_modify_error_count = 0;

            if (result == game::TASK_ERROR)
            {
                session_modify_error_count++;

                if (session_modify_error_count == SESSION_MODIFY_MAX_FAILURES)
                {
                    session_fix_print("Reached session modify failures limit, will attempt to recreate session...");
                    session_set_recreate();
                    session_modify_error_count = 0;
                }
            }
            else if (result == game::TASK_COMPLETE && session_modify_error_count > 0)
            {
                session_modify_error_count = 0;
            }

            return result;
        }

        void session_fix_watchdog()
        {
            static unsigned int reconnect_watch_count = 0;
            static bool reconnect_in_progress = false;

            game::DWOnlineStatus onlineStatus = game::dwGetLogOnStatus(0);

            if (reconnect)
            {
                if (reconnect_in_progress)
                {
                    if (onlineStatus != game::DW_LIVE_CONNECTED)
                    {
                        reconnect_watch_count++;

                        if (reconnect_watch_count == RECONNECT_MAX_WATCH)
                        {
                            session_fix_print("Failed to reconnect, trying again soon...");
                            reconnect_in_progress = false;
                            reconnect_watch_count = 0;
                        }
                    }
                    else
                    {
                        session_fix_print("Reconnection successful");
                        reconnect = false;
                        reconnect_in_progress = false;
                        reconnect_watch_count = 0;
                    }
                }
                else
                {
                    session_fix_print("Attempting to reconnect...");
                    reconnect_in_progress = true;
                    force_reconnect();
                }
            }

            if (!reconnect && (session_recreate && !session_recreate_in_progress) && (onlineStatus == game::DW_LIVE_CONNECTED))
            {
                const auto now = std::chrono::high_resolution_clock::now();
                const auto diff = now - session_recreate_time;

                // Wait a bit before attempting to recreate the session
                if (diff > 10000ms)
                {
                    session_recreate_in_progress = true;
                    session_fix_print("Recreating session...");
                    session_delete();
                    session_create();
                }
            }
        }
    }

    class component final : public component_interface
    {
    public:
        void post_unpack() override
        {
            // Disable Com_Error call if the session creation fails (Session_StartHostComplete+0x2A6)
            utils::hook::nop(0x8B3716, 5);

            // Disable Com_Error call in Live_DemonwareDisconnectCleanup just in case (Live_DemonwareDisconnectCleanup+0x4A)
            utils::hook::nop(0x468E3A, 5);

            // Redirect Com_DPrintf to Com_Printf for more verbosity
            utils::hook::jump(reinterpret_cast<uintptr_t>(game::Com_DPrintf.get()), game::Com_Printf);

            // Hook dwCloseRemoteTask call to retrieve DW error code (dwUpdateSessionComplete+0xB0)
            utils::hook::call(0x53EB20, dwCloseRemoteTask_stub);

            Session_ModifyComplete_hook.create(game::Session_ModifyComplete, Session_ModifyComplete);

            // Avoid executing map_rotate after a reconnect attempt, it still works if executed from ExitLevel or from the server console, don't worry
            Cbuf_AddText_hook.create(game::Cbuf_AddText, Cbuf_AddText_stub);

            Session_StartHost_hook.create(game::Session_StartHost, Session_StartHost_stub);
            Session_StartHostComplete_hook.create(game::Session_StartHostComplete, Session_StartHostComplete_stub);
            Session_Modify_hook.create(game::Session_Modify, Session_Modify_stub);
            Session_UpdateNetCode_hook.create(game::Session_UpdateNetCode, Session_UpdateNetCode_stub);

            scheduler::loop(session_fix_watchdog, scheduler::async, 15000ms, true);

            session_fix_print("Loaded!");
            session_fix_print("SESSION_MODIFY_MAX_FAILURES: %d, SESSION_CREATE_MAX_FAILURES: %d, RECONNECT_MAX_WATCH: %d", SESSION_MODIFY_MAX_FAILURES, SESSION_CREATE_MAX_FAILURES, RECONNECT_MAX_WATCH);
        }
    };
}

REGISTER_COMPONENT(session_fix::component)
