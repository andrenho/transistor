#ifndef COMPONENT_HH
#define COMPONENT_HH

#include <memory>
#include <vector>

#include "engine/types.hh"
#include "engine/componentdb/componentdef.hh"
#include "engine/geometry/direction.hh"
#include "engine/geometry/position.hh"

struct Component {
    ComponentDefinition const*    def;
    Direction                     rotation = Direction::N;
    std::unique_ptr<uint8_t[]>    data = nullptr;
    std::unique_ptr<bus_data_t[]> pins = nullptr;

    std::vector<std::pair<uintpin_t, Position>> pin_positions(Position const& component_pos) const;

    void      on_click() { def->on_click(*this); }
};

#endif //COMPONENT_HH
