#include "transistor.hh"

#include "loader.hh"

Transistor::Transistor()
    : L(luaL_newstate())
{
    luaL_openlibs(L);
    load_transistor(L);
}

Transistor::~Transistor()
{
    lua_close(L);
}
