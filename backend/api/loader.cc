#include "loader.hh"

#include <stdexcept>
#include <string>
using namespace std::string_literals;

void load_transistor(lua_State* L)
{
#ifndef NDEBUG   // dev mode
    if (luaL_dofile(L, "engine/contrib/tl.lua") != LUA_OK)
        throw std::runtime_error("Error loading tl.lua: "s + lua_tostring(L, -1));
#else            // release mode
#endif
}
