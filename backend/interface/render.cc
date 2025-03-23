#include "render.hh"

#include <cassert>
#include <string>
using namespace std::string_literals;

void Render::load_from_lua(lua_State* L)
{
    assert(lua_istable(L, -1));  // is render object

    // load scenes
    lua_getfield(L, -1, "scenes");
    assert(lua_istable(L, -1));

    // iterate through scenes
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        assert(lua_istable(L, -1));
        Scene scene;

        // iterate through instructions
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            assert(lua_istable(L, -1));
            add_instruction(L, scene);
            lua_pop(L, 1);
        }

        scenes.emplace_back(std::move(scene));
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // scenes
}

void Render::add_instruction(lua_State* L, Scene& scene)
{
    lua_getfield(L, -1, "command"); const char* command = lua_tostring(L, -1); lua_pop(L, 1);

    if (strcmp(command, "draw") == 0) {
        DrawInstruction di;
        lua_getfield(L, -1, "image"); di.image = lua_tostring(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "x"); di.x = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "y"); di.y = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "w"); di.w = lua_isnil(L, -1) ? 0 : lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "h"); di.h = lua_isnil(L, -1) ? 0 : lua_tointeger(L, -1); lua_pop(L, 1);
        // TODO - context
        scene.emplace_back(std::move(di));
    } else {
        throw std::runtime_error("Unknown command '"s + command + "'");
    }
}
