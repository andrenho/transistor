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
#include "engine/board/wire.lua.h"
#include "engine/board/geo/dir.lua.h"
#include "engine/board/geo/direction.lua.h"
#include "engine/board/geo/orientation.lua.h"
#include "engine/board/geo/position.lua.h"
#include "engine/board/geo/positionset.lua.h"
#include "engine/board/geo/rect.lua.h"
#include "engine/board/geo/pinpos.lua.h"
#include "engine/components/component.lua.h"
#include "engine/components/componentdb.lua.h"
#include "engine/components/componentdef.lua.h"
#include "engine/components/basic/button.lua.h"
#include "engine/components/basic/led.lua.h"
#include "engine/components/basic/npn.lua.h"
#include "engine/components/basic/pnp.lua.h"
#include "engine/components/basic/vcc.lua.h"
#include "engine/components/gates/or_2i.lua.h"
#include "engine/graphics/graphics.lua.h"
#include "engine/graphics/context.lua.h"
#include "engine/tests/placement.lua.h"
#include "engine/tests/pinpositions.lua.h"
#include "engine/tests/positions.lua.h"

extern "C" { extern int luaopen_simulator(lua_State* L); }

static std::unordered_map<std::string, Bytecode> embedded_bytecode = {
#define LOAD(name) { #name, { engine_##name##_lua, engine_##name##_lua_sz } }
    LOAD(sandbox),
    LOAD(device),
    LOAD(devicedb),
    LOAD(board_board),
    LOAD(board_wire),
    LOAD(board_geo_dir),
    LOAD(board_geo_direction),
    LOAD(board_geo_orientation),
    LOAD(board_geo_pinpos),
    LOAD(board_geo_position),
    LOAD(board_geo_positionset),
    LOAD(board_geo_rect),
    LOAD(components_component),
    LOAD(components_componentdb),
    LOAD(components_componentdef),
    LOAD(components_basic_button),
    LOAD(components_basic_led),
    LOAD(components_basic_npn),
    LOAD(components_basic_pnp),
    LOAD(components_basic_vcc),
    LOAD(components_gates_or_2i),
    LOAD(graphics_graphics),
    LOAD(graphics_context),
    LOAD(tests_pinpositions),
    LOAD(tests_positions),
    LOAD(tests_placement),
#undef LOAD
};

void load_transistor(lua_State* L)
{
    // save old require
    lua_getglobal(L, "require");
    lua_setglobal(L, "old_require");

    // override `require`
    lua_pushcfunction(L, [](lua_State* LL) {
        int top = lua_gettop(LL);

        const char* name = luaL_checkstring(LL, 1);
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

        // name == "simulator"
        /* TODO
        if (strcmp(name, "simulator") == 0) {
            luaopen_simulator(LL);
            cache();
            return 1;
        }
        */

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
