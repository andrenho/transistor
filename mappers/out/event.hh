#ifndef EVENT_HH
#define EVENT_HH

#include "luaw.hh"

namespace luaobj {

// catch-all for any kind of event

struct Event {
    std::string                type;
    std::optional<int>         x, y;
    std::optional<std::string> button;
    std::optional<std::string> key;
    std::optional<int>         index;

    void to_lua(lua_State* L) const {
        lua_newtable(L);
        luaw_setfield(L, -1, "type", type);
        luaw_setfield(L, -1, "x", x);
        luaw_setfield(L, -1, "y", y);
        luaw_setfield(L, -1, "index", index);
        luaw_setfield(L, -1, "button", button);
        luaw_setfield(L, -1, "key", key);
    }

    static Event from_lua(lua_State* L, int index) {
        return {
            .type = luaw_getfield<std::string>(L, index, "type"),
            .x = luaw_getfield<std::optional<int>>(L, index, "x"),
            .y = luaw_getfield<std::optional<int>>(L, index, "y"),
            .button = luaw_getfield<std::optional<std::string>>(L, index, "button"),
            .key = luaw_getfield<std::optional<std::string>>(L, index, "key"),
            .index = luaw_getfield<std::optional<int>>(L, index, "index"),
        };
    }
};

}

#endif //EVENT_HH
