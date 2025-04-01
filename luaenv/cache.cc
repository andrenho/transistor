#include "cache.hh"

#include <pastel2d.hh>

#include "luaw.hh"

void setup_cache(lua_State* L)
{
    lua_pushcfunction(L, [](lua_State* L) {
        lua_pushinteger(L, ps::res::idx(luaL_checkstring(L, 1)));
        return 1;
    });
    lua_setglobal(L, "cache_image");
}
