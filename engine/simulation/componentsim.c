#include "componentsim.h"

#include <assert.h>
#include <lauxlib.h>
#include <pl_log.h>

#include "sandbox/sandbox.h"

#define FN_NAME "simulate_all_components"

ts_Result ts_component_sim_init(ts_Sandbox* sandbox)
{
    lua_State* L = sandbox->L;
    const char* lua_code = "function " FN_NAME "(list) for _,c in pairs(list) do c[1].simulate(c[2]) end end";
    if (luaL_dostring(L, lua_code))
        PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "lua error: %s", lua_tostring(L, -1));
    assert(lua_gettop(L) == 0);
    return TS_OK;
}

ts_Result ts_component_sim_prepare(lua_State* L)
{
    lua_newtable(L);
    assert(lua_gettop(L) == 1);
    return TS_OK;
}

ts_Result ts_component_sim_add_component(lua_State* L, ts_Component* component, int idx)
{
    if (component->def->c_simulate) {
        component->def->c_simulate(component);
    } else {
        assert(lua_gettop(L) == 1);
        lua_createtable(L, 2, 0);
        lua_rawgeti(L, LUA_REGISTRYINDEX, component->def->luaref); lua_rawseti(L, -2, 1);
        lua_rawgeti(L, LUA_REGISTRYINDEX, component->luaref); lua_rawseti(L, -2, 2);
        lua_rawseti(L, -2, idx);
        assert(lua_gettop(L) == 1);
    }
    return TS_OK;
}

ts_Result ts_component_sim_execute(ts_Sandbox const* sandbox)
{
    PL_TRACE("Executing simulation with Lua");
    lua_State* L = sandbox->L;
    lua_getglobal(L, FN_NAME);
    lua_pushvalue(L, -2);
    if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        PL_ERROR_RET(TS_LUA_FUNCTION_ERROR, "lua error: %s", lua_tostring(L, -1));
    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);
    return TS_OK;
}