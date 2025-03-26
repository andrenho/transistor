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

size_t Render::add_dialog(lua_State* L, Callback const* callback)
{
    Dialog dialog;

    lua_getfield(L, -1, "title"); dialog.title = lua_tostring(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "default_button"); dialog.default_button = lua_tointeger(L, -1); lua_pop(L, 1);

    lua_getfield(L, -1, "text");
        if (lua_type(L, -1) == LUA_TSTRING) {
            dialog.text = { lua_tostring(L, -1) };
        } else {
            lua_pushnil(L);
            while (lua_next(L, -2)) {
                dialog.text.emplace_back(lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
    lua_pop(L, 1);

    lua_getfield(L, -1, "type");
        std::string type = lua_tostring(L, -1);
        if (type == "information")
            dialog.type = Dialog::Type::Information;
        else if (type == "question")
            dialog.type = Dialog::Type::Question;
        else if (type == "warning")
            dialog.type = Dialog::Type::Warning;
        else if (type == "error")
            dialog.type = Dialog::Type::Error;
        else
            dialog.type = Dialog::Type::None;
    lua_pop(L, 1);

    lua_getfield(L, -1, "buttons");
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            dialog.buttons.emplace_back(lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    lua_pop(L, 1);

    dialog.show = false;
    dialog.callback = callback;

    dialogs.emplace_back(std::move(dialog));
    return dialogs.size() - 1;
}

std::vector<MenuItem> Render::load_menus(lua_State* L)
{
    assert(lua_istable(L, -1));

    std::vector<MenuItem> ret;

    lua_pushnil(L);
    while (lua_next(L, -2)) {
        MenuItem menu;
        lua_getfield(L, -1, "text"); menu.text = lua_tostring(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "callback"); if (!lua_isnil(L, -1)) menu.callback_ref = std::make_unique<LuaRef>(L); else lua_pop(L, 1);

        lua_getfield(L, -1, "ask_confirmation");
        if (!lua_isnil(L, -1))
            menu.confirmation_dialog = add_dialog(L, &menu.callback_ref);
        lua_pop(L, 1);

        lua_getfield(L, -1, "items");
        if (!lua_isnil(L, -1))
            menu.items = load_menus(L);
        lua_pop(L, 1);

        ret.emplace_back(std::move(menu));
        lua_pop(L, 1);
    }

    return ret;
}
