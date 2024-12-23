#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <unordered_set>

#include "engine/geometry/position.hh"
#include "engine/layout/pin.hh"

struct Connection {
    std::unordered_set<Pin>      pins;
    std::unordered_set<Position> wire;
    bus_data_t                   value = 0;
};

using Connections = std::vector<Connection>;

#endif //CONNECTION_HH
