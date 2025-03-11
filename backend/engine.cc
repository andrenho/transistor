#include "engine.hh"

#include <string>
#include <stdexcept>
using namespace std::string_literals;

#include "engine/util/strict.lua.h"
#include "engine/util/serialize.lua.h"
#include "engine/util/util.lua.h"
#include "engine/board.lua.h"
#include "engine/compiler.lua.h"
#include "engine/component.lua.h"
#include "engine/componentdb.lua.h"
#include "engine/componentdef.lua.h"
#include "engine/cursor.lua.h"
#include "engine/geometry.lua.h"
#include "engine/sandbox.lua.h"
#include "engine/wire.lua.h"


Engine::Engine()
    : L(luaL_newstate()), simulation_(L)
{
    luaL_openlibs(L);  // TODO - sandbox

    // load engine
#define LOAD(name) load(#name, engine_##name##_lua, engine_##name##_lua_sz);
    LOAD(util_strict)
    LOAD(util_serialize)
    LOAD(util_util)
    LOAD(board)
    LOAD(compiler)
    LOAD(component)
    LOAD(componentdb)
    LOAD(componentdef)
    LOAD(cursor)
    LOAD(geometry)
    LOAD(sandbox)
    LOAD(wire)
#undef LOAD

    /*
    // run tests
    execute("run_tests()");

    // create sandbox
    execute("create_sandbox()");
    lua_assert(lua_gettop() == 1);
    sandbox_ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
    */
}

Engine::~Engine()
{
    lua_close(L);
}

void Engine::start()
{
    simulation_.start();
    execute(false, "sandbox = Sandbox.new()");
    execute(true, "sandbox:add_board(20, 10)");
}

void Engine::execute(bool recompile, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(nullptr, 0, fmt, ap);
    char buf[n + 1];
    vsnprintf(buf, sizeof buf, fmt, ap);

    simulation_.pause();
    if (luaL_dostring(L, buf) != LUA_OK)
        throw std::runtime_error("Error executing '"s + buf + "': " + lua_tostring(L, -1));
    if (recompile)
        recompile_sandbox();
    simulation_.resume();
}

void Engine::load(const char* name, uint8_t const* bytecode, size_t sz)
{
    if (luaL_loadbuffer(L, (const char *) bytecode, sz, name) != LUA_OK)
        throw std::runtime_error("Error loading '"s + name + "' script: " + lua_tostring(L, -1));
    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
        throw std::runtime_error("Error running '"s + name + "' script: " + lua_tostring(L, -1));
}

void Engine::recompile_sandbox()
{
    if (luaL_dostring(L, "compiler.compile(sandbox)") != LUA_OK)
        throw std::runtime_error("Error compiling: "s + lua_tostring(L, -1));
}
