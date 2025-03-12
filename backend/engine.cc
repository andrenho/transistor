#include "engine.hh"

#include <cassert>
#include <cstring>

#include <string>
#include <stdexcept>
using namespace std::string_literals;

#include "compilation.hh"
#include "simulation/native.hh"

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

    register_native_array_function();

    // load engine
#define LOAD(name) load_bytecode(#name, engine_##name##_lua, engine_##name##_lua_sz);
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
    assert(lua_gettop(L) == 0);

    simulation_.pause();
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
    simulation_.resume();

    hydrate_snapshot_with_values(snapshot, result);
    return snapshot;
}

void Engine::register_native_array_function()
{
    struct NativeArray {
        size_t   sz;
        uint8_t* data;
    };

    // `NativeArray` metatable

    luaL_newmetatable(L, "NativeArray");
    luaL_setfuncs(L, (luaL_Reg[]) {

        { "__index", [](lua_State* LL) {

            if (lua_isnumber(LL, 2)) {
                NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
                lua_pushinteger(LL, native_array->data[lua_tointeger(LL, 2) - 1]);
                return 1;
            }

            if (lua_isstring(LL, 2)) {
                const char* function = lua_tostring(LL, 2);

                if (strcmp(function, "ptr") == 0) {
                    lua_pushcfunction(LL, [](lua_State* LLL) {
                        NativeArray* native_array = (NativeArray *) luaL_checkudata(LLL, 1, "NativeArray");
                        lua_pushinteger(LLL, (intptr_t) native_array->data);
                        return 1;
                    });
                    return 1;
                }

                luaL_error(LL, "Invalid method '%s'", function);
            }

            luaL_error(LL, "Function called with a invalid type.");
            return 0;
        } },

        { "__newindex", [](lua_State* LL) {
            NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
            native_array->data[luaL_checkinteger(LL, 2) - 1] = luaL_checkinteger(LL, 3) & 0xff;
            return 0;
        } },

        { "__len", [](lua_State* LL) {
            NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
            lua_pushinteger(LL, native_array->sz);
            return 1;
        } },

        { "__gc", [](lua_State* LL) {
            NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
            free(native_array->data);
            return 0;
        } },

        { nullptr, nullptr },
    }, 0);
    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);

    // `native_array` function
    lua_pushcfunction(L, [](lua_State* LL) -> int {
        int sz = luaL_checkinteger(LL, 1);
        NativeArray* native_array = (NativeArray *) lua_newuserdata(LL, sizeof(NativeArray));
        native_array->sz = sz;
        native_array->data = (uint8_t *) malloc(sz);
        memset(native_array->data, 0, sz);
        luaL_getmetatable(LL, "NativeArray");
        assert(!lua_isnil(LL, -1));
        lua_setmetatable(LL, -2);
        return 1;
    });
    lua_setglobal(L, "native_array");
    assert(lua_gettop(L) == 0);
}
