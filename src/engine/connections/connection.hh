#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <ostream>
#include <unordered_set>

#include "engine/geometry/position.hh"
#include "engine/layout/pin.hh"

struct Connection {
    std::unordered_set<Pin>      pins;
    std::unordered_set<Position> wires;
    bus_data_t                   value = 0;

    friend std::ostream& operator<<(std::ostream& os, Connection const& connection) {
        os << "Connection:\n";
        os << "  Pins:\n";
        for (auto const& pin: connection.pins)
            os << "    " << pin << "\n";
        os << "  Wires:\n";
        for (auto const& w: connection.wires)
            os << "   " << w << "\n";
        os << "  Value: " << (int) connection.value << "\n";
        return os;
    }
};

using Connections = std::vector<Connection>;

#endif //CONNECTION_HH
