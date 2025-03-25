#include "render.hh"

#include <cassert>
#include <string>
using namespace std::string_literals;

void Render::load_from_lua(lua_State* L)
{
    assert(lua_istable(L, -1));
    lua_getfield(L, -1, "menu");
    if (!lua_isnil(L, -1))
        menus = load_menus(L);
    lua_pop(L, 1);
}

std::vector<MenuItem> Render::load_menus(lua_State* L)
{
    assert(lua_istable(L, -1));

    std::vector<MenuItem> ret;

    lua_pushnil(L);
    while (lua_next(L, -2)) {
        MenuItem menu;
        lua_getfield(L, -1, "text"); menu.text = lua_tostring(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "ask_confirmation"); menu.ask_confirmation = lua_isnil(L, -1) ? std::optional<std::string> {} : lua_tostring(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "callback"); if (!lua_isnil(L, -1)) menu.callback_ref = LuaRef(L); else lua_pop(L, 1);

        lua_getfield(L, -1, "items");
        if (!lua_isnil(L, -1))
            menu.items = load_menus(L);
        lua_pop(L, 1);

        ret.emplace_back(std::move(menu));
        lua_pop(L, 1);
    }

    return ret;
}