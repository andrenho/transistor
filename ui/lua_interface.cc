#include "lua_interface.hh"

#include <cassert>
#include <pastel2d.hh>

int G_luaref = -1;

static int G_load_image_base64(lua_State* L)
{
    luaL_error(L, "`G_load_image_base64` not implemented");
    return 0;
}

static void G_context_from_lua(lua_State* L, ps::Context& context, int idx)
{
    luaL_checktype(L, idx, LUA_TTABLE);

    lua_pushnil(L);
    while (lua_next(L, idx) != 0) {
        const char* property = luaL_checkstring(L, -2);
        if (strcmp(property, "position") == 0) {
            luaL_checktype(L, -1, LUA_TTABLE);
            if (lua_objlen(L, -1) != 2 && lua_objlen(L, -1) != 4)
                luaL_error(L, "Wrong number of parameters.");
            lua_rawgeti(L, -1, 1); context.position.x = luaL_checkint(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); context.position.y = luaL_checkint(L, -1); lua_pop(L, 1);
            if (lua_objlen(L, -1) > 2) {
                lua_rawgeti(L, -1, 3); context.position.w = luaL_checkint(L, -1); lua_pop(L, 1);
                lua_rawgeti(L, -1, 4); context.position.h = luaL_checkint(L, -1); lua_pop(L, 1);
            }
        } else if (strcmp(property, "rotation") == 0) {
            context.rotation = luaL_checknumber(L, -1);
        } else if (strcmp(property, "zoom") == 0) {
            context.zoom = luaL_checknumber(L, -1);
        } else if (strcmp(property, "opacity") == 0) {
            context.opacity = luaL_checknumber(L, -1);
        } else if (strcmp(property, "rotation_center") == 0) {
            luaL_checktype(L, -1, LUA_TTABLE);
            if (lua_objlen(L, -1) != 2)
                luaL_error(L, "Wrong number of parameters.");
            SDL_FPoint point;
            lua_rawgeti(L, -1, 1); point.x = luaL_checkint(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); point.y = luaL_checkint(L, -1); lua_pop(L, 1);
            context.rotation_center = point;
        }
        lua_pop(L, 1);
    }
}

static int G_render_image(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "__scene_ptr");
    luaL_checktype(L, -1, LUA_TLIGHTUSERDATA);
    ps::Scene* scene = (ps::Scene *) lua_touserdata(L, -1);
    lua_pop(L, 1);

    const char* image = luaL_checkstring(L, 2);
    int x = luaL_checkint(L, 3);
    int y = luaL_checkint(L, 4);

    ps::res::idx_t img;
    try {
        img = ps::res::idx(image);
    } catch (std::exception& e) {
        luaL_error(L, "error running 'render': %s", e.what());
        return 0;
    }

    ps::Context context;
    if (lua_gettop(L) >= 5)
        G_context_from_lua(L, context, 5);
    scene->add_image(img, (SDL_Rect) { .x=x, .y=y, .w=0, .h=0 }, context);

    return 0;
}

void initialize_lua_G_object(lua_State* L)
{
    static const luaL_Reg graph_obj[] = {
        { "load_image_base64", G_load_image_base64 },
        { "render_image",      G_render_image },
        { nullptr, nullptr },
    };
    lua_newtable(L);
    luaL_setfuncs(L, graph_obj, 0);

    G_luaref = luaL_ref(L, LUA_REGISTRYINDEX);
}

void G_set_scene(lua_State* L, ps::Scene* scene)
{
    int top = lua_gettop(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, G_luaref);
    lua_pushlightuserdata(L, scene);
    lua_setfield(L, -2, "__scene_ptr");
    lua_pop(L, 1);
    assert(lua_gettop(L) == top);
}
