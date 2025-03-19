#include <string.h>

#include <lua.h>
#include <lauxlib.h>

typedef struct Simulator {
} Simulator;

static int simulator_new(lua_State* L)
{
    Simulator* simulator = lua_newuserdata(L, sizeof(Simulator));
    memset(simulator, 0, sizeof(Simulator));
    return 1;
}

int luaopen_simulator(lua_State* L)
{
    static const luaL_Reg simulator_reg[] = {
        { "new", simulator_new },
        { NULL, NULL },
    };
    luaL_newlib(L, simulator_reg);
    return 1;
}