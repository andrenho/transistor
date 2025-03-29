#include "apifunction.hh"

ApiFunction ApiFunction::parse(lua_State* L)
{
    ApiFunction callback;
    lua_getfield(L, -1, "command"); callback.command = lua_tostring(L, -1); lua_pop(L, 1);
    return callback;
}
