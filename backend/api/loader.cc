#include "loader.hh"

#include <stdexcept>
#include <string>
using namespace std::string_literals;

//
// dev mode
//

#ifndef NDEBUG

void load_transistor(lua_State* L)
{
    if (luaL_dostring(L, R"(
        package.path = package.path .. ';./engine/?.lua;./backend/engine/?.lua'
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
#include "engine/board.lua.h"
#include "engine/sandbox.lua.h"

static std::unordered_map<std::string, Bytecode> engine_lua = {
#define LOAD(name) { #name, { engine_##name##_lua, engine_##name##_lua_sz } }
    LOAD(sandbox),
    LOAD(board),
#undef LOAD
};

void load_transistor(lua_State* L)
{
    // override `require`
    lua_pushcfunction(L, [](lua_State* LL) {
        const char* name = luaL_checkstring(LL, 1);
        auto it = engine_lua.find(name);
        if (it != engine_lua.end()) {
            int top = lua_gettop(LL);
            if (luaL_loadbuffer(LL, (const char *) it->second.data, it->second.sz, name) != LUA_OK)
                luaL_error(LL, "Error loading script `%s`: %s", name, lua_tostring(LL, -1));
            if (lua_pcall(LL, 0, LUA_MULTRET, 0) != LUA_OK)
                luaL_error(LL, "Error running script `%s`: %s", name, lua_tostring(LL, -1));
            return lua_gettop(LL) - top;  // TODO
        }
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
