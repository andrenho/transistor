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
        package.path = package.path .. ';./backend/?.lua'
        local tl = require 'engine.contrib.tl'
        tl.loader()
        require 'engine.sandbox'
    )") != LUA_OK)
        throw std::runtime_error("Error loading `transistor`: "s + lua_tostring(L, -1));

    // TODO - watch files
}

//
// release mode
//

#else

#include <vector>

struct Bytecode {
    const char* name;
    uint8_t const* data;
    size_t sz;
};

#include "engine/sandbox.lua.h"

static std::vector<Bytecode> engine_lua = {
#define LOAD(name) { #name, engine_##name##_lua, engine_##name##_lua_sz }
    LOAD(sandbox),
#undef LOAD
};

void load_transistor(lua_State* L)
{
    for (auto const& b : engine_lua) {
        if (luaL_loadbuffer(L, (const char *) b.data, b.sz, b.name) != LUA_OK)
            throw std::runtime_error("Error loading '"s + b.name + "' script: " + lua_tostring(L, -1));
        if (lua_pcall(L, 0, 0, 0) != LUA_OK)
            throw std::runtime_error("Error running '"s + b.name + "' script: " + lua_tostring(L, -1));
    }
}

#endif
