#include "engine.hh"

#include <cassert>
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

Engine::Engine()
    : L(luaL_newstate()), simulation_(L)
{
    luaL_openlibs(L);  // TODO - sandbox

    register_native_array_function();

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

    execute(false, "sandbox = Sandbox.new()");
    execute(true, "sandbox:add_board(20, 10)");
}

Engine::~Engine()
{
    lua_close(L);
}

void Engine::execute(bool recompile, const char* fmt, ...)
{
    assert(lua_gettop(L) == 0);

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
    if (luaL_dostring(L, "return compiler.snapshot(sandbox, compiler.compile(sandbox))") != LUA_OK)
        throw std::runtime_error("Error compiling: "s + lua_tostring(L, -1));
    assert(lua_gettop(L) == 1);

    CompilationResult result;

    // get components
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

    /*
    lua_pushnil(L);
    while (lua_next(L, -2)) {

        Connection connection;


        lua_pop(L, 1);
    }
    */

    lua_pop(L, 1);

    simulation_.update_compilation_result(std::move(result));
}

void Engine::register_native_array_function()
{
    // `NativeArray` metatable
    luaL_newmetatable(L, "NativeArray");
    luaL_setfuncs(L, (luaL_Reg[]) {
        { "ptr", [](lua_State* LL) {
            void* ptr = luaL_checkudata(LL, 1, "NativeArray");
            lua_pushinteger(LL, (intptr_t) ptr);
            return 1;
        } },
        { nullptr, nullptr },
    }, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
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
