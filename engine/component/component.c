#include "component.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <lauxlib.h>
#include <pl_log.h>

#include "componentdef.h"
#include "component_mt.h"
#include "sandbox/sandbox.h"
#include "util/serialize.h"

ts_Result call_component_lua_function(ts_Component* component, const char* function)
{
    PL_DEBUG("Calling lua function '%s' on component '%s'", function, component->def->key);

    lua_State* L = component->def->sandbox->L;
    assert(lua_gettop(L) == 0);

    lua_rawgeti(L, LUA_REGISTRYINDEX, component->def->luaref);
    lua_getfield(L, -1, function);
    if (!lua_isnil(L, -1)) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, component->luaref);
        int r = lua_pcall(L, 1, 0, 0);
        if (r != LUA_OK) {
            const char* error = lua_tostring(L, -1);
            lua_pop(L, 2);
            PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "lua function '%s' error: %s", function, error);
        }
    } else {
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    assert(lua_gettop(L) == 0);
    return TS_OK;
}

static void ts_component_create_lua_reference_object(ts_Component* component)
{
    lua_State* L = component->def->sandbox->L;
    ts_components_mt_init(L);

    // component
    lua_newtable(L);

    // def
    lua_rawgeti(L, LUA_REGISTRYINDEX, component->def->luaref);
    lua_setfield(L, -2, "def");

    // data
    LuaComponentData* data = lua_newuserdata(L, sizeof(LuaComponentData));
    data->component = component;
    luaL_getmetatable(L, "Data");
    lua_setmetatable(L, -2);
    lua_setfield(L, -2, "data");

    // pin
    LuaComponentData* pins = lua_newuserdata(L, sizeof(LuaComponentData));
    pins->component = component;
    luaL_getmetatable(L, "Pin");
    lua_setmetatable(L, -2);
    lua_setfield(L, -2, "pin");

    component->luaref = luaL_ref(L, LUA_REGISTRYINDEX);
    PL_DEBUG("Created lua reference object for component '%s': %d", component->def->key, component->luaref);

    call_component_lua_function(component, "init_component");

    assert(lua_gettop(L) == 0);
}

ts_Result ts_component_init(ts_Component* component, ts_ComponentDef const* def, ts_Direction direction)
{
    memset(component, 0, sizeof(ts_Component));
    component->def = def;
    component->direction = direction;
    component->data = calloc(1, def->data_size);
    component->pins = calloc(def->n_pins, sizeof component->pins[0]);

    ts_component_create_lua_reference_object(component);

    PL_DEBUG("Component '%s' initialized", component->def->key);
    return TS_OK;
}

ts_Result ts_component_finalize(ts_Component* component)
{
    luaL_unref(component->def->sandbox->L, LUA_REGISTRYINDEX, component->luaref);
    free(component->data);
    free(component->pins);
    PL_DEBUG("Component '%s' finalized", component->def->key);
    return TS_OK;
}

ts_Result ts_component_update_pos(ts_Component* component, ts_Board const* board, ts_Position pos)
{
    component->board = board;
    component->position = pos;
    PL_DEBUG("Component '%s' position updated to %d,%d", component->def->key, pos.x, pos.y);
    return TS_OK;
}

ts_Result ts_component_on_click(ts_Component* component)
{
    return call_component_lua_function(component, "on_click");
}

ts_Rect ts_component_rect(ts_Component const* component)
{
    return ts_component_def_rect(component->def, component->position, component->direction);
}

size_t ts_component_pin_positions(ts_Component const* component, ts_PinPos* pin_pos, size_t max_pin_pos)
{
    return ts_component_def_pin_positions(component->def, component->position, component->direction, pin_pos, max_pin_pos);
}

int ts_component_serialize(ts_Component const* component, int vspace, FILE* f)
{
    SR_INIT_INLINE("{");
    SR_CONT("  name = '%s',", component->def->key);
    SR_CONT("  direction = '%s',", ts_direction_serialize(component->direction));
    if (component->def->data_size > 0) {
        SR_CONT_INLINE("  data = {");
        for (size_t i = 0; i < component->def->data_size; ++i)
            fprintf(f, " 0x%02x,", component->data[i]);
        fprintf(f, " },\n");
    }
    SR_CONT_INLINE("}");
    return TS_OK;
}

ts_Result ts_component_unserialize(ts_Component* component, lua_State* L, ts_Sandbox* sb)
{
    // direction
    ts_Direction dir;
    lua_getfield(L, -1, "direction"); ts_direction_unserialize(&dir, L); lua_pop(L, 1);

    // setup component
    lua_getfield(L, -1, "name");
    const char* name = luaL_checkstring(L, -1);
    ts_Result r = ts_component_init(component, ts_component_db_def(&sb->component_db, name), dir);
    lua_pop(L, 1);
    if (r != TS_OK)
        return r;

    // update data
    lua_getfield(L, -1, "data");
    if (!lua_isnil(L, -1)) {
        component->data = realloc(component->data, component->def->data_size);
        for (size_t i = 0; i < lua_objlen(L, -1); ++i) {
            lua_rawgeti(L, -1, i + 1);
            component->data[i] = luaL_checkinteger(L, -1);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);

    return TS_OK;
}
