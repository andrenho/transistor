#include <lua.h>
#include <lauxlib.h>

static const luaL_Reg simulator_reg[] = {
    { NULL, NULL },
};

int luaopen_simulator(lua_State* L)
{
    luaL_newlib(L, simulator_reg);
    return 1;
}