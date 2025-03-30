#ifndef LUAOBJ_MENU_HH
#define LUAOBJ_MENU_HH

#include <luaw.hh>

#include "dialog.hh"
#include "callback.hh"

namespace luaobj {

struct MenuItem {
    std::string             text;
    std::optional<Dialog>   ask_confirmation;
    std::optional<Callback> callback;
    std::vector<MenuItem>   items;

    static MenuItem from_lua(lua_State* L, int index) {
        return {
            .text = luaw_getfield<std::string>(L, index, "text"),
            .ask_confirmation = luaw_getfield<std::optional<Dialog>>(L, index, "ask_confirmation"),
            .callback = luaw_getfield<std::optional<Callback>>(L, index, "callback"),
            .items = luaw_getfield<std::optional<std::vector<MenuItem>>>(L, index, "items").value_or(std::vector<MenuItem> {}),
        };
    }
};

}

#endif
