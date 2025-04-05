#include "compiled_circuit.hh"

#include <luaw.hh>

namespace luaobj {

Component Component::from_lua(lua_State* L, int index)
{
    return {
        .data = luaw_getfield<Array *>(L, index, "data"),
        .pins = luaw_getfield<Array *>(L, index, "pins"),
        .simulate = nullptr,  // TODO
    };
}

Pin Pin::from_lua(lua_State* L, int index)
{
    Pin pin = {
        .pin_value_ptr = nullptr,
        .pin_no = luaw_getfield<size_t>(L, index, "pin_no"),
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
    return {
        .pins = luaw_getfield<std::vector<Pin>>(L, index, "pins"),
    };
}

CompiledCircuit CompiledCircuit::from_lua(lua_State* L, int index)
{
    return CompiledCircuit {
        .connections = luaw_getfield<std::vector<Connection>>(L, index, "connections"),
        .components = luaw_getfield<std::vector<Component>>(L, index, "components"),
    };
}

}