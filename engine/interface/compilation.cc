#include "compilation.hh"

#include <cassert>
#include <lua.hpp>

CompilationResult parse_compilation_result(lua_State* L)
{
    CompilationResult result;

    assert(lua_gettop(L) == 1);
    assert(lua_istable(L, -1));

    // components

    lua_getfield(L, -1, "components");
    assert(lua_istable(L, -1));
    lua_pushnil(L);
    while (lua_next(L, -2)) {

        // create component
        CompilationResult::Component component {};
        lua_rawgeti(L, -1, 1); std::string key = lua_tostring(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 2); component.data = (uint8_t *) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 3); component.pins = (uint8_t *) lua_tointeger(L, -1); lua_pop(L, 1);
        lua_rawgeti(L, -1, 4); bool simulate_in_c = lua_toboolean(L, 1); lua_pop(L, 1);
        if (simulate_in_c) {
            auto it = native_functions.find(key);
            if (it != native_functions.end())
                component.simulate = it->second;
        }
        result.components.emplace_back(std::move(component));

        // store component by id
        lua_rawgeti(L, -1, 5); ComponentId id = lua_tointeger(L, -1); lua_pop(L, 1);
        result.component_by_id[id] = result.components.size() - 1;

        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    // connections

    lua_getfield(L, -1, "connections");
    assert(lua_istable(L, -1));
    lua_pushnil(L);
    while (lua_next(L, -2)) {
        assert(lua_istable(L, -1));

        CompilationResult::Connection connection;

        // pins
        lua_getfield(L, -1, "pins");
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            CompilationResult::Pin pin;
            lua_rawgeti(L, -1, 1); pin.pins = (uint8_t *) lua_tointeger(L, -1); lua_pop(L, 1);
            lua_rawgeti(L, -1, 2); pin.pin_no = lua_tointeger(L, -1) - 1; lua_pop(L, 1);
            lua_rawgeti(L, -1, 3); pin.dir = lua_tostring(L, -1)[0] == 'i' ? CompilationResult::PinDirection::Input : CompilationResult::PinDirection::Output; lua_pop(L, 1);
            connection.pins.emplace_back(std::move(pin));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        result.connections.emplace_back(std::move(connection));

        // wires
        lua_getfield(L, -1, "wires");
        size_t len = lua_objlen(L, -1);
        size_t connection_idx = result.connections.size() - 1;
        for (int i = 0; i < len; ++i) {
            lua_rawgeti(L, -1, i + 1);
            WireId wire_id = lua_tointeger(L, -1);
            result.connection_by_wire_id[wire_id] = connection_idx;
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    assert(lua_gettop(L) == 1);

    return result;
}
