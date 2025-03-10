#include "component_mt.h"

#include <assert.h>
#include <lauxlib.h>
#include <pl_log.h>

#include "component.h"
#include "componentdef.h"

static ts_Component* get_component(lua_State* L)
{
    return ((LuaComponentData *) lua_touserdata(L, 1))->component;
}

static int data_get(lua_State* L)
{
    lua_pushinteger(L, get_component(L)->data[luaL_checkinteger(L, 2) - 1]);
    return 1;
}

static int data_set(lua_State* L)
{
    get_component(L)->data[luaL_checkinteger(L, 2) - 1] = luaL_checkinteger(L, 3) & 0xff;
    return 0;
}

static int data_len(lua_State* L)
{
    lua_pushinteger(L, get_component(L)->def->data_size);
    return 1;
}

static int pin_get(lua_State* L)
{
    lua_pushinteger(L, get_component(L)->pins[luaL_checkinteger(L, 2) - 1]);
    return 1;
}

static int pin_set(lua_State* L)
{
    get_component(L)->pins[luaL_checkinteger(L, 2) - 1] = luaL_checkinteger(L, 3) & 0xff;
    return 0;
}

static int pin_len(lua_State* L)
{
    lua_pushinteger(L, get_component(L)->def->n_pins);
    return 1;
}

void ts_components_mt_init(lua_State* L)
{
    luaL_getmetatable(L, "Data");
    if (!lua_isnil(L, -1)) {
        lua_pop(L, 1);
        return;
    }
    lua_pop(L, 1);

    luaL_newmetatable(L, "Data");
    lua_pushcfunction(L, data_get); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, data_set); lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, data_len); lua_setfield(L, -2, "__len");
    lua_pop(L, 1);

    luaL_newmetatable(L, "Pin");
    lua_pushcfunction(L, pin_get); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, pin_set); lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, pin_len); lua_setfield(L, -2, "__len");
    lua_pop(L, 1);

    assert(lua_gettop(L) == 0);
    PL_DEBUG("Component Lua metadata created.");
}
