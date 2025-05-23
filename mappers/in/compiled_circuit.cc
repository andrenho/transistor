#include "compiled_circuit.hh"

#include <cassert>
#include <luaw.hh>

namespace luaobj {

Component Component::from_lua(lua_State* L, int index)
{
    lua_getfield(L, index, "def");
    auto key = luaw_getfield<std::string>(L, -1, "key");
    lua_pop(L, 1);

    SimFunc simulate = nullptr;
    auto it = native_functions.find(key);
    if (it != native_functions.end())
        simulate = it->second;

    return {
        .data = luaw_getfield<Array *>(L, index, "data"),
        .pins = luaw_getfield<Array *>(L, index, "pins"),
        .simulate = simulate,
    };
}

Pin Pin::from_lua(lua_State* L, int index)
{
    Pin pin = {
        .pin_value_ptr = nullptr,
        .pin_no = luaw_getfield<size_t>(L, index, "pin_no") - 1,
        .dir = PinDirection::Input
    };

    if (luaw_getfield<std::string>(L, index, "dir") == "output")
        pin.dir = PinDirection::Output;

    lua_getfield(L, index, "component");
    pin.pin_value_ptr = &luaw_getfield<Array *>(L, index, "pins")->data[pin.pin_no];
    lua_pop(L, 1);

    return pin;
}

Connection Connection::from_lua(lua_State* L, int index)
{
    std::vector<uint32_t> wire_pos_hashes;
    luaw_getfield(L, index, "wires");
    luaw_getfield(L, index, "items");
    luaw_pairs(L, -1, [&wire_pos_hashes](lua_State* L) {
        wire_pos_hashes.push_back(lua_tointeger(L, -2));  // key = position hash, we don't care about the value
    });
    lua_pop(L, 2);

    return {
        .pins = luaw_getfield<std::vector<Pin>>(L, index, "pins"),
        .wire_pos_hashes = std::move(wire_pos_hashes),
    };
}

CompiledCircuit CompiledCircuit::from_lua(lua_State* L, int index)
{
    CompiledCircuit cc {
        .connections = luaw_getfield<std::vector<Connection>>(L, index, "connections"),
        .components = luaw_getfield<std::vector<Component>>(L, index, "components"),
    };
    for (auto const& connection: cc.connections)
        cc.total_wires += connection.wire_pos_hashes.size();
    return cc;
}

}