#include "components.h"

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <lauxlib.h>

#include "board.h"
#include "button.lua.h"
#include "graphics.h"

//
// CONTEXT OBJECT
//

// TODO

//
// GRAPHICS OBJECT
//

typedef struct GraphicsObject {
    ps_Scene* scene;
} GraphicsObject;

static int G_luaref = -1;

static int graphics_load_image_base64(lua_State* L)
{
    // TODO
    return 0;
}

static int graphics_render_image(lua_State* L)
{
    const char* image = luaL_checkstring(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);

    ps_res_idx_t img = ps_res_idx(image);

    // ps_scene_add_image_with(scene, img, (SDL_Rect) { x, y }, CTX_END);

    return 0;
}

static void create_graphics_object(lua_State* L)
{
    GraphicsObject* go = lua_newuserdata(L, sizeof(GraphicsObject));
    memset(go, 0, sizeof(GraphicsObject));

    static const struct luaL_Reg graph_obj[] = {
        { "load_image_base64", graphics_load_image_base64 },
        { "render_image",      graphics_render_image },
        { NULL, NULL }
    };
    luaL_newmetatable(L, "Graphics");
    luaL_setfuncs(L, graph_obj, 0);

    lua_setmetatable(L, -2);
    G_luaref = luaL_ref(L, -1);
}

//
// COMPONENT OPERATIONS
//

static void load_component(ts_Transistor* t, const char* lua_code)
{
    ts_Result r = ts_transistor_component_db_add_from_lua(t, lua_code, G_luaref);
    if (r != TS_OK) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading default component", ts_last_error(&t->sandbox, NULL),
            ps_graphics_window());
        abort();
    }
}

void component_render(ts_Transistor const* T, ts_ComponentSnapshot const* component, ps_Scene* scene)
{
    ts_transistor_component_render(T, component, G_luaref, component->pos.x * TILE_SIZE, component->pos.y * TILE_SIZE);
}

//
// INIT
//

void components_init(ts_Transistor* t)
{
    create_graphics_object(t->sandbox.L);
    load_component(t, components_button_lua);
}
