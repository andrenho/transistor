#ifndef EVENT_HH
#define EVENT_HH

#include "luaw.hh"

namespace luaobj {

// catch-all for any kind of event

struct Event {
    struct KeyMod {
        bool ctrl, alt, shift;

        void to_lua(lua_State* L) const {
            lua_createtable(L, 0, 3);
            luaw_setfield(L, -1, "ctrl", ctrl, false);
            luaw_setfield(L, -1, "alt", alt, false);
            luaw_setfield(L, -1, "shift", shift, false);
        }

        static KeyMod from_lua(lua_State* L, int index) {
            return {
                .ctrl = luaw_getfield<bool>(L, index, "ctrl"),
                .alt = luaw_getfield<bool>(L, index, "alt"),
                .shift = luaw_getfield<bool>(L, index, "shift"),
            };
        }
    };

    std::string                type;
    std::optional<int>         x, y;
    std::optional<std::string> button;
    std::optional<std::string> key;
    std::optional<KeyMod>      mod;
    std::optional<int>         index;

    void to_lua(lua_State* L) const {
        lua_createtable(L, 0, 7);
        luaw_setfield(L, -1, "type", type);
        luaw_setfield(L, -1, "x", x);
        luaw_setfield(L, -1, "y", y);
        luaw_setfield(L, -1, "button", button);
        luaw_setfield(L, -1, "key", key);
        luaw_setfield(L, -1, "mod", mod);
        luaw_setfield(L, -1, "index", index);
    }

    static Event from_lua(lua_State* L, int index) {
        return {
            .type = luaw_getfield<std::string>(L, index, "type"),
            .x = luaw_getfield<std::optional<int>>(L, index, "x"),
            .y = luaw_getfield<std::optional<int>>(L, index, "y"),
            .button = luaw_getfield<std::optional<std::string>>(L, index, "button"),
            .key = luaw_getfield<std::optional<std::string>>(L, index, "key"),
            .mod = luaw_getfield<std::optional<KeyMod>>(L, index, "mod"),
            .index = luaw_getfield<std::optional<int>>(L, index, "index"),
        };
    }
};

}

#endif //EVENT_HH
