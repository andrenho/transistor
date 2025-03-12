#ifndef CONNECTION_HH
#define CONNECTION_HH

#include "simulation/native.hh"

#include <vector>

struct Component {
    uint8_t* data;
    uint8_t* pins;
    SimFunc  simulate;
};

struct Connection {
};

struct CompilationResult {
    std::vector<Component> components;
    std::vector<Connection> connections;
};

#endif //CONNECTION_HH
