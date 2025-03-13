#ifndef CONNECTION_HH
#define CONNECTION_HH

#include "simulation/native.hh"

#include <lua.hpp>
#include <vector>

typedef long ComponentId;
typedef long WireId;

struct CompilationResult {
    struct Component {
        uint8_t* data = nullptr;
        uint8_t* pins = nullptr;
        SimFunc  simulate = nullptr;
    };

    enum class PinDirection { Input, Output };

    struct Pin {
        uint8_t*     pins = nullptr;
        size_t       pin_no = 0;
        PinDirection dir = PinDirection::Input;
    };

    struct Connection {
        std::vector<Pin>    pins {};
        uint8_t             value = 0;
    };

    std::vector<Component> components {};
    std::vector<Connection> connections {};

    std::unordered_map<ComponentId, size_t> component_by_id;         // value = index in the component vector
    std::unordered_map<WireId, size_t>      connection_by_wire_id;   // value = index in the connection vector
};

CompilationResult parse_compilation_result(lua_State* L);

#endif //CONNECTION_HH
