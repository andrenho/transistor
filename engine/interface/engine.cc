#include "engine.hh"

#include <cassert>
#include <cstring>

#include <string>
#include <stdexcept>

#include "native_array.hh"
using namespace std::string_literals;

#include "compilation.hh"
#include "simulation/native.hh"

// embedded resources
#include "util/strict.lua.h"
#include "util/serialize.lua.h"
#include "util/util.lua.h"
#include "board.lua.h"
#include "compiler.lua.h"
#include "component.lua.h"
#include "componentdb.lua.h"
#include "componentdef.lua.h"
#include "cursor.lua.h"
#include "geometry.lua.h"
#include "sandbox.lua.h"
#include "wire.lua.h"

static const char* lua_init = R"(
    bit = require("bit")
    bnot = bit.bnot
    band, bor, bxor = bit.band, bit.bor, bit.bxor
    lshift, rshift, rol = bit.lshift, bit.rshift, bit.rol
)";

Engine::Engine()
    : L(luaL_newstate()), simulation_(L)
{
    luaL_openlibs(L);

    if (luaL_dostring(L, lua_init) != LUA_OK)
        throw std::runtime_error("Error executing Lua initialization: "s + lua_tostring(L, -1));

    register_native_array(L);

    // load engine
#define LOAD(name) load_bytecode(#name, engine_engine_##name##_lua, engine_engine_##name##_lua_sz);
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

    simulation_.pause();
    simulation_.resume();
}

Engine::~Engine()
{
    simulation_.stop();
    lua_close(L);
}

void Engine::start()
{
    execute("sandbox = Sandbox.new()", false);
    execute("return function() sandbox:simulate_lua_components() end", false);
    simulation_.set_simulate_luaref(luaL_ref(L, LUA_REGISTRYINDEX));
    execute("sandbox:add_board(20, 10)");
    simulation_.start();
}

void Engine::load_bytecode(const char* name, uint8_t const* bytecode, size_t sz)
{
    if (luaL_loadbuffer(L, (const char *) bytecode, sz, name) != LUA_OK)
        throw std::runtime_error("Error loading '"s + name + "' script: " + lua_tostring(L, -1));
    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
        throw std::runtime_error("Error running '"s + name + "' script: " + lua_tostring(L, -1));
}

void Engine::execute(std::string const& command, bool recompile, std::function<void()> const& and_also_do)
{
    simulation_.pause();

    assert(lua_gettop(L) == 0);

    if (luaL_dostring(L, command.c_str()) != LUA_OK)
        throw std::runtime_error("Error executing '"s + command + "': " + lua_tostring(L, -1));
    if (recompile)
        recompile_sandbox();
    if (and_also_do)
        and_also_do();

    simulation_.resume();
}

void Engine::recompile_sandbox()
{
    if (luaL_dostring(L, "return compiler.snapshot(sandbox, compiler.compile(sandbox))") != LUA_OK)
        throw std::runtime_error("Error compiling: "s + lua_tostring(L, -1));

    auto result = parse_compilation_result(L);
    lua_pop(L, 1);

    simulation_.update_compilation_result(std::move(result));
}

Snapshot Engine::take_snapshot()
{
    simulation_.pause();
    if (luaL_dostring(L, "return sandbox:take_snapshot('scene')") != LUA_OK)
        throw std::runtime_error("Error taking snapshot: "s + lua_tostring(L, -1));

    auto snapshot = parse_snapshot(L);
    lua_pop(L, 1);

    auto result = simulation_.result();
    hydrate_snapshot_with_values(snapshot, result);

    simulation_.resume();
    return snapshot;
}
