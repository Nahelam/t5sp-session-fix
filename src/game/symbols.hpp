#pragma once

#include "game.hpp"

#define WEAK __declspec(selectany)

namespace game
{
    // Functions

    WEAK symbol <int(char* string, unsigned int count, const char* format, char* ap)> _vsnprintf{ 0x965D30 };

    WEAK symbol<void(int channel, const char* fmt, ...)> Com_Printf{ 0x43BF30 };
    WEAK symbol<void(int channel, const char* fmt, ...)> Com_DPrintf{ 0x60F7D0 };

    WEAK symbol <void(int localClientNum, const char* text)> Cbuf_AddText{ 0x49B930 };

    WEAK symbol <void(overlappedTask* task)> TaskManager_ClearTask{ 0x6765A0 };

    WEAK symbol <bdLobbyErrorCode(void* _this)> bdRemoteTask_getErrorCode{ 0x935BB0 };

    WEAK symbol <taskCompleteResults(overlappedTask* const task)> dwUpdateSessionComplete{ 0x53EA70 };
    WEAK symbol <void(overlappedTask* task)> dwCloseRemoteTask{ 0x6426A0 };
    WEAK symbol <void(const bdLobbyErrorCode code, char* const buffer, const unsigned int bufferSize)> dwLobbyErrorCodeToString{ 0x5B3770 };
    WEAK symbol <DWOnlineStatus(const int localControllerIndex)> dwGetLogOnStatus{ 0x6677C0 };

    WEAK symbol <void(SessionData_s* session, int sessionFlags, int numPrivateSlots, int numPublicSlots)> Session_StartHost{ 0x570520 };
    WEAK symbol <taskCompleteResults(const int slot)> Session_StartHostComplete{ 0x8B3470 };
    WEAK symbol <void(const int localControllerIndex, SessionData_s* session, const int flags, const int publicSlots, const int privateSlots)> Session_Modify{ 0x611930 };
    WEAK symbol <taskCompleteResults(/* const int slot (in eax) */)> Session_ModifyComplete{ 0x8B31D0 };
    WEAK symbol <void(SessionData_s* session)> Session_DeleteSession{ 0x654A00 };
    WEAK symbol <void(const int netCodeVersion)> Session_UpdateNetCode{ 0x5285F0 };     // Guessed name

    WEAK symbol <int(PartyData_s* party)> Party_AreWeHost{ 0x573790 };
    WEAK symbol <void(PartyData_s* party)> PartyHost_GamestateChanged{ 0x43EC30 };
    WEAK symbol <void(PartyData_s* party)> Party_ReAddAllPlayers{ 0x405BD0 };

    // Variables

    WEAK symbol<overlappedTask[32]> overlappedTasks_2{ 0x3839D40 };

    WEAK symbol<SessionData_s> g_serverSession{ 0x383A1C8 };
    WEAK symbol<PartyData_s> g_lobbyData{ 0x2F618B8 };

    WEAK symbol<bool> dwControllerData_forceLogOn{ 0x10CF408 };
    WEAK symbol<int> dwControllerData_logOnAttempts{ 0x10CF3E4 };

    WEAK symbol <dvar_s*> session_nonblocking{ 0x3879AC4 };
    WEAK symbol <dvar_s*> onlinegame{ 0x247FED0 };
}
