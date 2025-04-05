#ifndef COMPILATION_HH
#define COMPILATION_HH

#include <vector>

#include "luaenv/array.hh"
#include "simulator/native.hh"

namespace luaobj {

struct Component {
    Array* data;
    Array* pins;
    SimFunc  simulate;
    static Component from_lua(lua_State* L, int index);
};

enum class PinDirection { Input, Output };

struct Pin {
    uint8_t*     pin_value_ptr;
    size_t       pin_no;
    PinDirection dir;
    static Pin from_lua(lua_State* L, int index);
};

struct Connection {
    std::vector<Pin> pins;
    uint8_t          value = 0;
    static Connection from_lua(lua_State* L, int index);
};

struct CompiledCircuit {
    std::vector<Connection> connections;
    std::vector<Component> components;
    static CompiledCircuit from_lua(lua_State* L, int index);
};

}

#endif //COMPILATION_HH
