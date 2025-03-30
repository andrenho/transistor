#include "scenemapper.hh"

#include <cassert>
#include <string>
using namespace std;

#include "luaw.hh"

namespace scene_mapper {

static ps::Context map_to_context(lua_State* L)
{
    ps::Context context {};
    context.rotation = luaw_getfield<std::optional<float>>(L, -1, "rotation").value_or(0.f);
    context.opacity = luaw_getfield<std::optional<float>>(L, -1, "opacity").value_or(1.f);
    context.zoom = luaw_getfield<std::optional<float>>(L, -1, "zoom").value_or(1.f);

    lua_getfield(L, -1, "position"); if (!lua_isnil(L, -1)) {
        context.position = {
            .x = luaw_getfield<int>(L, -1, "x"),
            .y = luaw_getfield<int>(L, -1, "y"),
            .w = luaw_getfield<std::optional<int>>(L, -1, "w").value_or(0),
            .h = luaw_getfield<std::optional<int>>(L, -1, "h").value_or(0),
        };
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "rotation_center"); if (!lua_isnil(L, -1)) {
        context.rotation_center = {
            .x = luaw_getfield<float>(L, -1, "x"),
            .y = luaw_getfield<float>(L, -1, "y"),
        };
    }
    lua_pop(L, 1);

    return context;
}

static void map_to_instruction(lua_State* L, ps::Scene& scene)
{
    assert(lua_istable(L, -1));

    auto command = luaw_getfield<std::string>(L, -1, "command");

    if (command == "draw") {
        ps::Context context {};
        auto image = luaw_getfield<std::string>(L, -1, "image");
        int x = luaw_getfield<int>(L, -1, "x");
        int y = luaw_getfield<int>(L, -1, "y");
        int w = luaw_getfield<std::optional<int>>(L, -1, "w").value_or(0);
        int h = luaw_getfield<std::optional<int>>(L, -1, "h").value_or(0);
        lua_getfield(L, -1, "context"); if (!lua_isnil(L, -1)) context = map_to_context(L); lua_pop(L, 1);
        scene.add_image(image, SDL_Rect { x, y, w, h }, context);
    } else if (command == "push_context") {
        ps::Context context {};
        lua_getfield(L, -1, "context"); context = map_to_context(L); lua_pop(L, 1);
        scene.push_context(context);
    } else if (command == "pop_context") {
        scene.pop_context();
    } else {
        throw std::runtime_error("Unknown command '"s + command + "'");
    }
}

std::vector<ps::Scene> map_to_scene(lua_State* L)
{
    int top = lua_gettop(L);

    std::vector<ps::Scene> scenes;

    assert(lua_istable(L, -1));  // is scene object

    // iterate through scenes
    luaw_ipairs(L, -1, [&scenes](lua_State* L, int _) {
        ps::Scene& scene = scenes.emplace_back();
        luaw_ipairs(L, -1, [&scene](lua_State* L, int _) {
            map_to_instruction(L, scene);
        });
    });

    assert(lua_gettop(L) == top);
    return scenes;
}

}