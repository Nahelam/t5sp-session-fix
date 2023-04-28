#include <stdinc.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

BOOL APIENTRY DllMain(HMODULE /*module_*/, DWORD ul_reason_for_call, LPVOID /*reserved_*/)
{
    if (game::environment::is_sp)
    {
        if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        {
            component_loader::post_unpack();
        }

        if (ul_reason_for_call == DLL_PROCESS_DETACH)
        {
            component_loader::pre_destroy();
        }
    }

    return TRUE;
}
