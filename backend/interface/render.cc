#include "render.hh"

#include <cassert>
#include <string>
using namespace std::string_literals;

void Render::load_from_lua(lua_State* L)
{
    /*
    assert(lua_istable(L, -1));
    lua_getfield(L, -1, "menu");
    if (!lua_isnil(L, -1))
        load_menu(L, menu);
    lua_pop(L, 1);
    */
}

void Render::load_menu(lua_State* L, MenuItem& menu)
{
    lua_getfield(L, -1, "text"); menu.text = lua_tostring(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "ask_confirmation"); menu.ask_confirmation = lua_isnil(L, -1) ? std::optional<std::string> {} : lua_tostring(L, -1); lua_pop(L, 1);
    // lua_getfield(L, -1, "f"); if (!lua_isnil(L, -1)) menu.f_ref.set(); lua_pop(L, -1);

    // TODO - subitems
}