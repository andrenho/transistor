#ifndef COMPONENT_HH
#define COMPONENT_HH

#include <vector>
#include <utility>

#include "engine/types.hh"
#include "engine/componentdb/componentdef.hh"
#include "engine/geometry/direction.hh"
#include "engine/geometry/position.hh"

struct Component {
    ComponentDefinition* def;
    Direction            rotation = Direction::N;
    void*                data = nullptr;

    std::vector<std::pair<uintpin_t, Position>> pin_positions(Position const& component_pos) const;
};

#endif //COMPONENT_HH
