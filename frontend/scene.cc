#include "scene.hh"

#include <cassert>
#include <string>
using namespace std::string_literals;

void SceneMapper::render(lua_State* L)
{
    assert(lua_istable(L, -1));  // is render object

    // load scenes
    lua_getfield(L, -1, "scenes");
    assert(lua_istable(L, -1));

    // iterate through scenes
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        ps::Scene scene;
        assert(lua_istable(L, -1));

        // iterate through instructions
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            assert(lua_istable(L, -1));
            instruction(L, scene);
            lua_pop(L, 1);
        }

        scenes.emplace_back(std::move(scene));
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // scenes
}

ps::Context SceneMapper::context(lua_State* L)
{
    ps::Context context {};
    lua_getfield(L, -1, "rotation"); if (!lua_isnil(L, -1)) context.rotation = lua_tonumber(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "opacity"); if (!lua_isnil(L, -1)) context.opacity = lua_tonumber(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "zoom"); if (!lua_isnil(L, -1)) context.zoom = lua_tonumber(L, -1); lua_pop(L, 1);

    lua_getfield(L, -1, "position"); if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "x"); int x = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "y"); int y = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "w"); int w = lua_isnil(L, -1) ? 0 : lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "h"); int h = lua_isnil(L, -1) ? 0 : lua_tointeger(L, -1); lua_pop(L, 1);
        context.position = { x, y, w, h };
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "rotation_center"); if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "x"); float x = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "y"); float y = lua_tointeger(L, -1); lua_pop(L, 1);
        context.rotation_center = { x, y };
    }
    lua_pop(L, 1);

    return context;
}

void SceneMapper::instruction(lua_State* L, ps::Scene& scene)
{
    lua_getfield(L, -1, "command"); const char* command = lua_tostring(L, -1); lua_pop(L, 1);

    if (strcmp(command, "draw") == 0) {
        ps::Context context {};
        lua_getfield(L, -1, "image"); const char* image = lua_tostring(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "x"); int x = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "y"); int y = lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "w"); int w = lua_isnil(L, -1) ? 0 : lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "h"); int h = lua_isnil(L, -1) ? 0 : lua_tointeger(L, -1); lua_pop(L, 1);
        lua_getfield(L, -1, "context"); if (!lua_isnil(L, -1)) context = SceneMapper::context(L); lua_pop(L, 1);
        scene.add_image(image, SDL_Rect { x, y, w, h }, context);
    } else if (strcmp(command, "push_context") == 0) {
        ps::Context context {};
        lua_getfield(L, -1, "context"); context = SceneMapper::context(L); lua_pop(L, 1);
        scene.push_context(context);
    } else if (strcmp(command, "pop_context") == 0) {
        scene.pop_context();
    } else {
        throw std::runtime_error("Unknown command '"s + command + "'");
    }
}
