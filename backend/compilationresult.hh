#ifndef CONNECTION_HH
#define CONNECTION_HH

#include "simulation/native.hh"

#include <vector>

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
    std::vector<Pin> pins {};
    uint8_t          value = 0;
};

struct CompilationResult {
    std::vector<Component> components {};
    std::vector<Connection> connections {};
};

#endif //CONNECTION_HH
