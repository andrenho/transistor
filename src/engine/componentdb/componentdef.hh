#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH

#include <cstddef>
#include <functional>
#include <string>
#include "engine/types.hh"

struct Component;

struct ComponentDefinition {
    enum class Type { SingleTile, IC };

    std::string name;
    Type        type;
    bool        can_rotate;

    size_t      data_size = 0;
    std::function<void(Component&)>            on_click = nullptr;
    std::function<void(Component&)>            simulate = nullptr;

    constexpr uintpin_t pin_count() const
    {
        if (type == Type::SingleTile)
            return 4;
        throw std::runtime_error("IC type not yet supported for `pin_count`.");
    }

    // TODO - add IC fields (size, pins)
};

#endif //COMPONENTDEFINITION_HH
