#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <unordered_map>

#include "pin.hh"
#include "engine/board/wire.hh"
#include "engine/geometry/position.hh"

struct Layout {
    std::unordered_map<Position, Pin>  pins;
    std::unordered_map<Position, Wire> wires;

    friend std::ostream& operator<<(std::ostream& os, Layout const& layout) {
        os << "Layout:\n";
        os << "  Pins:\n";
        for (auto const& [pos, pin]: layout.pins)
            os << "    " << pin << "\n";
        os << "  Wires:\n";
        for (auto const& [pos, wire]: layout.wires)
            os << "   " << pos << "\n";
        return os;
    }
};

#endif //LAYOUT_HH
