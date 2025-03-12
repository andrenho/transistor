#include "engine.hh"

#include <cassert>
#include <cstring>

#include <string>
#include <stdexcept>
using namespace std::string_literals;

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
    assert(lua_gettop(L) == 1);

    CompilationResult result;

    // components

    assert(lua_istable(L, -1));
    lua_getfield(L, -1, "components");
    assert(lua_istable(L, -1));
    lua_pushnil(L);
    while (lua_next(L, -2)) {

        // find simulate function
        lua_rawgeti(L, -1, 1); std::string key = lua_tostring(L, -1); lua_pop(L, 1);
        auto it = native_functions.find(key);

        // if found, add component to list
        if (it != native_functions.end()) {

            Component component {};
            component.simulate = it->second;

            // find additional data
            lua_rawgeti(L, -1, 2); component.data = (uint8_t *) lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 3); component.pins = (uint8_t *) lua_tointeger(L, -1); lua_pop(L, 1);

            result.components.emplace_back(std::move(component));
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    // connections

    lua_getfield(L, -1, "connections");
    assert(lua_istable(L, -1));
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        assert(lua_istable(L, -1));

        Connection connection;

        // pins
        lua_getfield(L, -1, "pins");
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            Pin pin;
            lua_rawgeti(L, -1, 1); pin.pins = (uint8_t *) lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); pin.pin_no = lua_tointeger(L, -1) - 1; lua_pop(L, 1);
            lua_rawgeti(L, -1, 3); pin.dir = lua_tostring(L, -1)[0] == 'i' ? PinDirection::Input : PinDirection::Output; lua_pop(L, 1);
            connection.pins.emplace_back(std::move(pin));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        result.connections.emplace_back(std::move(connection));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);

    simulation_.update_compilation_result(std::move(result));
}

void Engine::register_native_array_function()
{
    // `NativeArray` metatable

    luaL_newmetatable(L, "NativeArray");
    luaL_setfuncs(L, (luaL_Reg[]) {

        { "__index", [](lua_State* LL) {
            uint8_t* data = (uint8_t *) lua_touserdata(LL, 1);

            if (lua_isnumber(LL, 2)) {
                lua_pushinteger(LL, data[lua_tointeger(LL, 2)]);
                return 1;
            }

            if (lua_isstring(LL, 2)) {
                const char* function = lua_tostring(LL, 2);

                if (strcmp(function, "ptr") == 0) {
                    lua_pushcfunction(LL, [](lua_State* LL) {
                        void* ptr = luaL_checkudata(LL, 1, "NativeArray");
                        lua_pushinteger(LL, (intptr_t) ptr);
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
            uint8_t* data = (uint8_t *) lua_touserdata(LL, 1);
            data[luaL_checkinteger(LL, 2)] = luaL_checkinteger(LL, 3) & 0xff;
            return 0;
        } },

        { nullptr, nullptr },
    }, 0);
    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);

    // `native_array` function
    lua_pushcfunction(L, [](lua_State* LL) -> int {
        int sz = luaL_checkinteger(LL, 1);
        void* data = lua_newuserdata(LL, sz);
        memset(data, 0, sz);
        luaL_getmetatable(LL, "NativeArray");
        assert(!lua_isnil(LL, -1));
        lua_setmetatable(LL, -2);
        return 1;
    });
    lua_setglobal(L, "native_array");
    assert(lua_gettop(L) == 0);
}
