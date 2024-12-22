#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <unordered_map>
#include <variant>

#include "pin.hh"
#include "engine/editor/wire.hh"
#include "engine/geometry/position.hh"

struct Layout {
    using Element = std::variant<Wire, Pin>;
    std::unordered_map<Position, Element> elements;
};

#endif //LAYOUT_HH
