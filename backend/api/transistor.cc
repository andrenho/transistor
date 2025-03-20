#include "transistor.hh"

#include "loader.hh"

Transistor::Transistor()
    : L(luaL_newstate())
{
    luaL_openlibs(L);
    load_transistor(L);

    lua_getglobal(L, "init");
    lua_call(L, 0, 0);
}

Transistor::~Transistor()
{
    lua_close(L);
}

void Transistor::run_tests()
{
    lua_getglobal(L, "run_tests");
    lua_call(L, 0, 0);
}
