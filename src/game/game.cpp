#include <stdinc.hpp>
#include "game.hpp"

namespace game
{
    gamemode current = *reinterpret_cast<int*>(0x401337) == 0x281488C0
        ? gamemode::multiplayer
        : gamemode::singleplayer;

    namespace environment
    {
        bool is_mp()
        {
            return current == gamemode::multiplayer;
        }

        bool is_sp()
        {
            return current == gamemode::singleplayer;
        }
    }
}
