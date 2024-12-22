#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <unordered_map>

#include "pin.hh"
#include "engine/editor/wire.hh"
#include "engine/geometry/position.hh"

struct Layout {
    std::unordered_map<Position, Pin>  pins;
    std::unordered_map<Position, Wire> wires;
};

#endif //LAYOUT_HH
