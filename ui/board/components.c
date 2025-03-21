#include "components.h"

#define NATIVE_COMPONENTS 1

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <lauxlib.h>

#include "board.h"
#include "graphics.h"

static int G_luaref = -1;

//
// RENDERING
//

static int graphics_load_image_base64(lua_State* L)
{
    // TODO
    return 0;
}


static void graphics_context_from_lua(ps_Context* context, lua_State* L, int idx)
{
    luaL_checktype(L, idx, LUA_TTABLE);

    lua_pushnil(L);
    while (lua_next(L, idx) != 0) {
        const char* property = luaL_checkstring(L, -2);
        if (strcmp(property, "position") == 0) {
            luaL_checktype(L, -1, LUA_TTABLE);
            if (lua_objlen(L, -1) != 2 && lua_objlen(L, -1) != 4)
                luaL_error(L, "Wrong number of parameters.");
            lua_rawgeti(L, -1, 1); context->position.x = luaL_checknumber(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); context->position.y = luaL_checknumber(L, -1); lua_pop(L, 1);
            if (lua_objlen(L, -1) > 2) {
                lua_rawgeti(L, -1, 3); context->position.w = luaL_checknumber(L, -1); lua_pop(L, 1);
                lua_rawgeti(L, -1, 4); context->position.h = luaL_checknumber(L, -1); lua_pop(L, 1);
            }
        } else if (strcmp(property, "rotation") == 0) {
            context->rotation = luaL_checknumber(L, -1);
        } else if (strcmp(property, "zoom") == 0) {
            context->zoom = luaL_checknumber(L, -1);
        } else if (strcmp(property, "opacity") == 0) {
            context->opacity = luaL_checknumber(L, -1);
        } else if (strcmp(property, "rotation_center") == 0) {
            luaL_checktype(L, -1, LUA_TTABLE);
            if (lua_objlen(L, -1) != 2)
                luaL_error(L, "Wrong number of parameters.");
            lua_rawgeti(L, -1, 1); context->rotation_center.x = luaL_checknumber(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); context->rotation_center.y = luaL_checknumber(L, -1); lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
}

static int graphics_render_image(lua_State* L)
{
    lua_getfield(L, 1, "__scene_ptr");
    ps_Scene* scene = lua_touserdata(L, -1);
    lua_pop(L, 1);

    const char* image = luaL_checkstring(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);

    ps_res_idx_t img = ps_res_idx(image);

    ps_Context context = ps_create_context();
    if (lua_gettop(L) >= 5)
        graphics_context_from_lua(&context, L, 5);
    ps_scene_add_image(scene, img, (SDL_Rect) { .x=x, .y=y }, &context);

    return 0;
}

static void component_renderer_setup_borrow(lua_State* L, void* p_scene)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, G_luaref);
    lua_pushlightuserdata(L, p_scene);
    lua_setfield(L, -2, "__scene_ptr");
    lua_pop(L, 1);
}

void component_renderer_setup(ts_Transistor* T, ps_Scene* scene)
{
    ts_borrow_lua(T, component_renderer_setup_borrow, scene);
}

void component_render(ts_Transistor* T, ts_ComponentSnapshot const* component)
{
    ts_component_render(T, component, component->pos.x * TILE_SIZE, component->pos.y * TILE_SIZE);
}

int G_init(lua_State* L)
{
    static const struct luaL_Reg graph_obj[] = {
        { "load_image_base64", graphics_load_image_base64 },
        { "render_image",      graphics_render_image },
        { NULL, NULL }
    };
    lua_newtable(L);
    luaL_setfuncs(L, graph_obj, 0);

    G_luaref = luaL_ref(L, LUA_REGISTRYINDEX);
    return G_luaref;
}
