#include "transistor.hh"

#include <string>
#include <stdexcept>
using namespace std::string_literals;

#include "loader.hh"

Transistor::Transistor()
    : L(luaL_newstate())
{
    luaL_openlibs(L);

    load_transistor(L);
    if (luaL_dostring(L, "require 'api'") != LUA_OK)
        throw std::runtime_error("Error loading Transistor API lua file: "s + lua_tostring(L, -1));

    lua_getglobal(L, "init");
    lua_call(L, 0, 0);
}

Transistor::~Transistor()
{
    lua_close(L);
}

bool Transistor::run_tests()
{
    lua_getglobal(L, "run_tests");
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        printf("Tests failed:\n%s\n", lua_tostring(L, -1));
        return false;
    }

    return true;
}
