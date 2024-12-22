#ifndef LAYOUT_HH
#define LAYOUT_HH

#include <unordered_map>
#include <variant>

#include "engine/editor/wire.hh"
#include "engine/editor/component.hh"
#include "engine/geometry/position.hh"

struct Layout {

    struct Pin {
        Component* component;
        uint8_t    pin_no;
    };

    using Element = std::variant<Wire, Pin>;
    std::unordered_map<Position, Element> elements;
};

#endif //LAYOUT_HH
