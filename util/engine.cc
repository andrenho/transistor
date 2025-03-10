#include "util/engine.hh"

#include <string>
#include <stdexcept>
using namespace std::string_literals;

#include "engine/engine.lua.h"
#include "engine/simulation.lua.h"

Engine::Engine()
    : L(luaL_newstate())
{
    luaL_openlibs(L);

    // load engine
    load("engine", engine_engine_lua, engine_engine_lua_sz);
    load("simulation", engine_simulation_lua, engine_simulation_lua_sz);

    // run tests
    execute("run_tests()");

    // create sandbox
    execute("create_sandbox()");
    lua_assert(lua_gettop() == 1);
    sandbox_ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
}

Engine::~Engine()
{
    lua_close(L);
}

void Engine::execute(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(nullptr, 0, fmt, ap);
    char buf[n + 1];
    vsnprintf(buf, sizeof buf, fmt, ap);

    if (luaL_dostring(L, buf) != LUA_OK)
        throw std::runtime_error("Error executing '"s + buf + "': " + lua_tostring(L, -1));
}

void Engine::load(const char* name, uint8_t const* bytecode, size_t sz)
{
    if (luaL_loadbuffer(L, (const char *) bytecode, sz, name) != LUA_OK)
        throw std::runtime_error("Error loading '"s + name + "' script: " + lua_tostring(L, -1));
    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
        throw std::runtime_error("Error running '"s + name + "' script: " + lua_tostring(L, -1));
}
