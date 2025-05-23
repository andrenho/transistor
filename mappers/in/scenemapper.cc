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
    context.draw_border = luaw_getfield<std::optional<bool>>(L, -1, "draw_border").value_or(false);

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
    // we're trying to be as most optimized as possible here

    assert(lua_istable(L, -1));

    lua_rawgeti(L, -1, 1); const char* command = lua_tostring(L, -1); lua_pop(L, 1);

    if (strcmp(command, "draw") == 0) {
        ps::Context context {};
        lua_rawgeti(L, -1, 3); int x = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 4); int y = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 5); int w = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 6); int h = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 7); if (!lua_isnil(L, -1)) context = map_to_context(L); lua_pop(L, 1);

        // image
        lua_rawgeti(L, -1, 2);
        if (lua_type(L, -1) == LUA_TSTRING)
            scene.add_image(lua_tostring(L, -1), SDL_Rect { x, y, w, h }, context);
        else if (lua_type(L, -1) == LUA_TNUMBER)
            scene.add_image(lua_tointeger(L, -1), SDL_Rect { x, y, w, h }, context);
        lua_pop(L, 1);
    } else if (strcmp(command, "text") == 0) {
        ps::Context context {};
        lua_rawgeti(L, -1, 3); int font_sz = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 4); const char* text = lua_tostring(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 5); int x = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 6); int y = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 7); int w = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 8); int h = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 9); uint8_t r = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 10); uint8_t g = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 11); uint8_t b = (int) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 12); const char* align_s = lua_tostring(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 13); if (!lua_isnil(L, -1)) context = map_to_context(L); lua_pop(L, 1);

        ps::TextAlignment align = ps::TextAlignment::Left;
        if (strcmp(align_s, "center") == 0)
            align = ps::TextAlignment::Center;
        else if (strcmp(align_s, "right") == 0)
            align = ps::TextAlignment::Right;

        // font
        lua_rawgeti(L, -1, 2);
        if (lua_type(L, -1) == LUA_TSTRING)
            scene.add_text(lua_tostring(L, -1), text, SDL_Rect { x, y, w, h }, font_sz, SDL_Color { r, g, b, 255 }, align, context);
        else
            scene.add_text(lua_tointeger(L, -1), text, SDL_Rect { x, y, w, h }, font_sz, SDL_Color { r, g, b, 255 }, align, context);
        lua_pop(L, 1);

    } else if (strcmp(command, "push_context") == 0) {
        ps::Context context {};
        lua_rawgeti(L, -1, 2); if (!lua_isnil(L, -1)) context = map_to_context(L); lua_pop(L, 1);
        scene.push_context(context);
    } else if (strcmp(command, "pop_context") == 0) {
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
        lua_getfield(L, -1, "instructions");
        luaw_ipairs(L, -1, [&scene](lua_State* L, int _) {
            map_to_instruction(L, scene);
        });
        lua_pop(L, 1);
    });

    assert(lua_gettop(L) == top);
    return scenes;
}

}