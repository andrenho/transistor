#include "transistor.hh"

#include <string>
#include <stdexcept>
using namespace std::string_literals;

#include "../contrib/dmon.h"

extern void setup_require(lua_State* L);
extern void setup_array(lua_State* L);

Transistor::Transistor()
    : L(luaL_newstate())
{
    luaL_openlibs(L);

    setup();

#ifdef DEV
    // watch for files and restart simulation
    dmon_init();
    dmon_watch("backend/engine",
        [](dmon_watch_id watch_id, dmon_action action, const char* dirname, const char* filename, const char* oldname, void* user) {
        }, DMON_WATCHFLAGS_RECURSIVE | DMON_WATCHFLAGS_FOLLOW_SYMLINKS, this);
#endif
}

Transistor::~Transistor()
{
    dmon_deinit();
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

void Transistor::setup()
{
#ifdef DEV
    if (luaL_dostring(L, R"(
        package.path = package.path .. ';./backend/?.lua;./engine/?.lua;./backend/engine/?.lua;./engine/?.d.tl;./backend/engine/?.d.tl'
        package.cpath = package.cpath .. ';./sim/?.so'
        serpent = require 'contrib.serpent'
        local tl = require 'contrib.tl'
        tl.loader()
    )") != LUA_OK)
        throw std::runtime_error("Error initializing tl: "s + lua_tostring(L, -1));
#else
    setup_require(L);
#endif

    setup_array(L);

    if (luaL_dostring(L, "require 'api'") != LUA_OK)
        throw std::runtime_error("Error loading Transistor API lua file: "s + lua_tostring(L, -1));
}