#ifndef EVENT_HH
#define EVENT_HH

#include "luaw.hh"

namespace luaobj {

// catch-all for any kind of event

struct Event {
    std::string                type;
    std::optional<int>         x, y;
    std::optional<int>         index;
    std::optional<std::string> button;
    std::optional<std::string> key;

    void to_lua(lua_State* L) const {
        lua_newtable(L);
        luaw_setfield(L, -1, "type", type);
        luaw_setfield(L, -1, "x", x);
        luaw_setfield(L, -1, "y", y);
        luaw_setfield(L, -1, "index", index);
        luaw_setfield(L, -1, "button", button);
        luaw_setfield(L, -1, "key", key);
    }
};

}

#endif //EVENT_HH
