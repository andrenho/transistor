#include "engine.hh"

#include <cassert>
#include <cstring>

#include <string>
#include <stdexcept>
using namespace std::string_literals;

#include "native_array.hh"
#include "compilation.hh"
#include "embedded.hh"

static const char* lua_init = R"(
    RUNNING_FROM_C = true

    bit = require("bit")
    bnot = bit.bnot
    band, bor, bxor = bit.band, bit.bor, bit.bxor
    lshift, rshift, rol = bit.lshift, bit.rshift, bit.rol
)";

Engine::Engine()
    : L(luaL_newstate()), simulation_(L)
{
    luaL_openlibs(L);

    // initialization code
    if (luaL_dostring(L, lua_init) != LUA_OK)
        throw std::runtime_error("Error executing Lua initialization: "s + lua_tostring(L, -1));

    // create NativeArray
    register_native_array(L);

    // load engine
    for (auto const& bytecode : engine_lua)
        load_bytecode(bytecode.name, bytecode.data, bytecode.sz);
    register_load_all_components_function();   // replace function
}

Engine::~Engine()
{
    simulation_.stop();
    lua_close(L);
}

void Engine::register_load_all_components_function() const
{
    lua_getglobal(L, "ComponentDB");
    assert(lua_istable(L, -1));
    lua_pushcfunction(L, [](lua_State* LL) {
        int top = lua_gettop(LL);
        lua_pushvalue(LL, 1);            // ComponentDB  [self]
        for (auto const& comp: components_lua) {
            lua_getfield(LL, -1, "add"); //  .add(
            lua_pushvalue(LL, 1);        //     self,
            if (luaL_loadbuffer(LL, (const char*) comp.data, comp.sz, comp.name) != LUA_OK  // bytecode
                    || lua_pcall(LL, 0, 1, 0) != LUA_OK)
                luaL_error(LL, "Error loading component '%s': %s", comp.name, lua_tostring(LL, 1));
            lua_pushboolean(LL, true);   //     true,  [native]
            lua_call(LL, 3, 0);
        }
        lua_pop(LL, 1);
        assert(lua_gettop(LL) == top);
        return 0;
    });
    lua_setfield(L, -2, "load_all_native_components");
    lua_pop(L, -1);
}


void Engine::start()
{
    execute("sandbox = Sandbox.new()", false);
    execute("return function() sandbox:simulate_lua_components() end", false);
    simulation_.set_simulate_luaref(luaL_ref(L, LUA_REGISTRYINDEX));
    execute("sandbox:add_board(20, 10)");
    simulation_.start();
}

void Engine::load_bytecode(const char* name, uint8_t const* bytecode, size_t sz) const
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
