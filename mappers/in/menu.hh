#ifndef LUAOBJ_MENU_HH
#define LUAOBJ_MENU_HH

#include <luaw.hh>

#include "dialog.hh"
#include "mappers/out/event.hh"

namespace luaobj {

struct MenuItem {
    std::string             text;
    std::optional<Dialog>   ask_confirmation;
    std::optional<Event>    event;
    std::vector<MenuItem>   items;

    static MenuItem from_lua(lua_State* L, int index) {
        return {
            .text = luaw_getfield<std::string>(L, index, "text"),
            .ask_confirmation = luaw_getfield<std::optional<Dialog>>(L, index, "ask_confirmation"),
            .event = luaw_getfield<std::optional<Event>>(L, index, "event"),
            .items = luaw_getfield<std::optional<std::vector<MenuItem>>>(L, index, "items").value_or(std::vector<MenuItem> {}),
        };
    }
};

}

#endif
