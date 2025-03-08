#include "components.h"

#define NATIVE_COMPONENTS 1

#include <stdlib.h>
#include <SDL3/SDL.h>
#include <lauxlib.h>

#include "board.h"
#include "graphics.h"

// embedded Lua files
#include "components/basic/button.lua.h"
#include "components/basic/led.lua.h"
#include "components/basic/npn.lua.h"
#include "components/basic/pnp.lua.h"
#include "components/basic/vcc.lua.h"
#include "components/gates/or_2i.lua.h"

static int G_luaref = -1;

//
// COMPONENT OPERATIONS
//

static void load_component(ts_Transistor* t, const char* lua_code)
{
    ts_component_db_add_from_lua(t, lua_code, G_luaref);
}

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
    ps_scene_add_image(scene, img, (SDL_Rect) { x, y }, &context);

    return 0;
}

static void create_graphics_object(lua_State* L, void* data)
{
    static const struct luaL_Reg graph_obj[] = {
        { "load_image_base64", graphics_load_image_base64 },
        { "render_image",      graphics_render_image },
        { NULL, NULL }
    };
    lua_newtable(L);
    luaL_setfuncs(L, graph_obj, 0);

    G_luaref = luaL_ref(L, LUA_REGISTRYINDEX);
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
    ts_component_render(T, component, G_luaref, component->pos.x * TILE_SIZE, component->pos.y * TILE_SIZE);
}

//
// INITIALIZATION
//

static void button_sim(ts_Component* button)
{
    button->pins[0] = button->pins[1] = button->pins[2] = button->pins[3] = button->data[0];
}

static void led_sim(ts_Component* led)
{
    led->data[0] = led->pins[0] | led->pins[1] | led->pins[2] | led->pins[3];
}

static void vcc_sim(ts_Component* vcc)
{
    vcc->pins[0] = vcc->pins[1] = vcc->pins[2] = vcc->pins[3] = 1;
}

static void npn_sim(ts_Component* npn)
{
    static const size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
    npn->pins[OUT] = npn->pins[IN] & (npn->pins[SWITCH_1] | npn->pins[SWITCH_2]);
}

static void pnp_sim(ts_Component* pnp)
{
    static const size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
    pnp->pins[OUT] = pnp->pins[IN] & !(pnp->pins[SWITCH_1] | pnp->pins[SWITCH_2]);
}

static void or_2i_sim(ts_Component* c)
{
    static const size_t IN0 = 0, IN1 = 0, Q_ = 2, Q = 3;
    c->pins[Q] = c->pins[IN0] | c->pins[IN1];
    c->pins[Q_] = !c->pins[Q];
}

void components_init(ts_Transistor* t)
{
    ts_borrow_lua(t, create_graphics_object, NULL);

    load_component(t, components_basic_button_lua);
    load_component(t, components_basic_led_lua);
    load_component(t, components_basic_npn_lua);
    load_component(t, components_basic_pnp_lua);
    load_component(t, components_basic_vcc_lua);
    load_component(t, components_gates_or_2i_lua);

#if NATIVE_COMPONENTS
    ts_component_db_native_simulation(t, "__button", button_sim);
    ts_component_db_native_simulation(t, "__led", led_sim);
    ts_component_db_native_simulation(t, "__npn", npn_sim);
    ts_component_db_native_simulation(t, "__pnp", pnp_sim);
    ts_component_db_native_simulation(t, "__vcc", vcc_sim);
    ts_component_db_native_simulation(t, "__or_2i", or_2i_sim);
#endif
}
