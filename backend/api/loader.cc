#include "loader.hh"

#include <cstdint>
#include <cstring>

#include <algorithm>
#include <string>
#include <stdexcept>
using namespace std::string_literals;

//
// dev mode
//

#ifndef NDEBUG

void load_transistor(lua_State* L)
{
    if (luaL_dostring(L, R"(
        package.path = package.path .. ';./backend/?.lua;./engine/?.lua;./backend/engine/?.lua'
        package.cpath = package.cpath .. ';./sim/?.so'
        serpent = require 'contrib.serpent'
        local tl = require 'contrib.tl'
        tl.loader()
    )") != LUA_OK)
        throw std::runtime_error("Error initializing tl: "s + lua_tostring(L, -1));
    if (luaL_dofile(L, "./api/api.lua") != LUA_OK)
        throw std::runtime_error("Error loading Transistor API lua file: "s + lua_tostring(L, -1));
}

//
// release mode
//

#else

#include <unordered_map>

struct Bytecode {
    uint8_t const* data;
    size_t sz;
};

#include "api/api.lua.h"
#include "engine/sandbox.lua.h"
#include "engine/device.lua.h"
#include "engine/devicedb.lua.h"
#include "engine/board/board.lua.h"

extern "C" { extern int luaopen_simulator(lua_State* L); }

static std::unordered_map<std::string, Bytecode> embedded_bytecode = {
#define LOAD(name) { #name, { engine_##name##_lua, engine_##name##_lua_sz } }
    LOAD(sandbox),
    LOAD(device),
    LOAD(devicedb),
    LOAD(board_board),
#undef LOAD
};

void load_transistor(lua_State* L)
{
    // override `require`
    lua_pushcfunction(L, [](lua_State* LL) {
        int top = lua_gettop(LL);

        const char* name = luaL_checkstring(LL, 1);
        std::string cached_name = "__require_"s + name;

        // look for cached version
        lua_getglobal(LL, cached_name.c_str());
        if (!lua_isnil(LL, -1))
            return 1;
        lua_pop(LL, 1);

        // cache module function
        auto cache = [&]() {
            int n_results = lua_gettop(LL) - top;
            if (n_results == 0)
                lua_newtable(LL);
            else if (n_results == 1)
                lua_pushvalue(LL, -1);
            else
                luaL_error(LL, "Modules returning >1 result not supported.");
            lua_setglobal(LL, cached_name.c_str());
        };

        // name == "simulator"
        if (strcmp(name, "simulator") == 0) {
            luaopen_simulator(LL);
            cache();
            return 1;
        }

        // name in `embedded_bytecode`
        std::string key = name;
        std::replace(key.begin(), key.end(), '.', '_');
        auto it = embedded_bytecode.find(key);
        if (it != embedded_bytecode.end()) {
            if (luaL_loadbuffer(LL, (const char *) it->second.data, it->second.sz, name) != LUA_OK)
                luaL_error(LL, "Error loading script `%s`: %s", name, lua_tostring(LL, -1));
            if (lua_pcall(LL, 0, LUA_MULTRET, 0) != LUA_OK)
                luaL_error(LL, "Error running script `%s`: %s", name, lua_tostring(LL, -1));
            cache();
            return lua_gettop(LL) - top;
        }

        // name not found
        luaL_error(LL, "Script `%s` not found embedded in the binary.", name);
        return 0;
    });
    lua_setglobal(L, "require");

    // run Lua API
    if (luaL_loadbuffer(L, (const char *) api_api_lua, api_api_lua_sz, "api") != LUA_OK)
        throw std::runtime_error("Error loading Lua API: "s + lua_tostring(L, -1));
    if (lua_pcall(L, 0, 0, 0) != LUA_OK)
        throw std::runtime_error("Error running Lua API: "s + lua_tostring(L, -1));
}

#endif
