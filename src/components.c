#include "components.h"

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <lauxlib.h>

#include "button.lua.h"
#include "graphics.h"

//
// CONTEXT OBJECT
//

// TODO

//
// GRAPHICS OBJECT
//

static int G_luaref = -1;

static int graphics_load_image_base64(lua_State* L)
{
    return 0;
}

static int graphics_render_image(lua_State* L)
{
    return 0;
}

static void create_graphics_object(lua_State* L)
{
    static const struct luaL_Reg graph_obj[] = {
        { "load_image_base64", graphics_load_image_base64 },
        { "render_image",      graphics_render_image },
        { NULL, NULL }
    };
    lua_newtable(L);
    luaL_setfuncs(L, graph_obj, 0);
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

//
// INIT
//

void components_init(ts_Transistor* t)
{
    create_graphics_object(t->sandbox.L);
    load_component(t, components_button_lua);
}

//
// INFORMATION
//
int graphics_luaref()
{
    return G_luaref;
}
