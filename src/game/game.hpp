#pragma once

#include "structs.hpp"

namespace game
{
    enum gamemode
    {
        none,
        multiplayer,
        singleplayer
    };

    extern gamemode current;

    namespace environment
    {
        bool is_mp();
        bool is_sp();
    }

    template <typename T>
    class symbol
    {
    public:
        symbol(const size_t address)
            : object_(reinterpret_cast<T*>(address))
        {
        }

        T* get() const
        {
            return object_;
        }

        operator T* () const
        {
            return this->get();
        }

        T* operator->() const
        {
            return this->get();
        }

    private:
        T* object_;
    };
}

#include "symbols.hpp"
