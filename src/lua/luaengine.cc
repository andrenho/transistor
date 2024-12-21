#include "luaengine.hh"

#include <lua.hpp>

#include "battery/embed.hpp"

LuaEngine::LuaEngine()
    : L(luaL_newstate())
{
    luaL_openlibs(L);

    lua_pushstring(L, "main");

    auto script = b::embed<"resources/scripts/hello.lua">();
    if (luaL_loadbuffer(L, script.str().c_str(), script.length(), PROJECT_NAME) != LUA_OK)
        throw std::runtime_error("Error loading Lua script.");
    lua_call(L, 0, 1);

    lua_settable(L, LUA_REGISTRYINDEX);
}

LuaEngine::~LuaEngine()
{
    lua_close(L);
}

void LuaEngine::hello() const
{
    lua_pushstring(L, "main");
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, "hello_from_lua");
    lua_gettable(L, -2);

    lua_call(L, 0, 0);

    lua_pop(L, 1);
}
