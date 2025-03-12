#include "compilation.hh"

#include <cassert>
#include <lua.hpp>

CompilationResult parse_compilation_result(lua_State* L)
{
    CompilationResult result;

    assert(lua_gettop(L) == 1);

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

    return result;
}
