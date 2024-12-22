#ifndef COMPONENT_HH
#define COMPONENT_HH

#include "engine/componentdb/componentdef.hh"
#include "engine/geometry/direction.hh"

struct Component {
    ComponentDefinition* def;
    Direction            rotation = Direction::N;
    void*                data = nullptr;
};

#endif //COMPONENT_HH
