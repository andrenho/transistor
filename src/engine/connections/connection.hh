#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <vector>

#include "engine/types.hh"
#include "engine/geometry/position.hh"
#include "engine/layout/pin.hh"

struct Connection {
    std::vector<Pin>      pins;
    std::vector<Position> tiles;
    bus_data_t                 value = 0;
};

using Connections = std::vector<Connection>;

#endif //CONNECTION_HH
