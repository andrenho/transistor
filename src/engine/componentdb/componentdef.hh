#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH

#include <cstddef>
#include <functional>
#include <string>

#include "engine/types.hh"
#include "ui/scene.hh"

class  ResourceManager;
class  Scene;
struct Resource;
struct Component;

struct ComponentDefinition {
    enum class Type { SingleTile, IC };

    std::string name;
    Type        type;
    bool        can_rotate;

    size_t      data_size = 0;

    std::function<void(Component& component)>                                   on_click = [](Component&) {};
    std::function<void(Component& component)>                                   simulate = [](Component&) {};
    std::function<void(Component const& component, Scene& scene, int x, int y)> render = [](Component const&, Scene&, int, int) {};
    std::function<std::vector<uintpin_t>(Component const& component)>           input_pins = [](Component const&) { return std::vector<uintpin_t>{}; };

    constexpr uintpin_t pin_count() const
    {
        if (type == Type::SingleTile)
            return 4;
        throw std::runtime_error("IC type not yet supported for `pin_count`.");
    }

    // TODO - add IC fields (size, pins)
};

#endif //COMPONENTDEFINITION_HH
