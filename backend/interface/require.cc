// replace Lua `require` with a version that loads embedded files

#include <string>
using namespace std::string_literals;

#include <lua.hpp>

#include "embedded.hh"

void setup_require(lua_State* L)
{
    // save old require
    lua_getglobal(L, "require");
    lua_setglobal(L, "old_require");

    // override `require`
    lua_pushcfunction(L, [](lua_State* LL) {

        int top = lua_gettop(LL);

        const char* name = luaL_checkstring(LL, 1);
        std::string key = name;
        std::string cached_name = "__require_"s + name;

        // is it "bit"?
        if (strcmp(name, "bit") == 0) {
            lua_getglobal(LL, "old_require");
            lua_pushstring(LL, name);
            lua_call(LL, 1, 1);
            return 1;
        }

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

        if (key == "decl.array")
            return 0;

        // name == "simulator"
        /* TODO
        if (strcmp(name, "simulator") == 0) {
            luaopen_simulator(LL);
            cache();
            return 1;
        }
        */

        // name in `embedded_bytecode`
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
}