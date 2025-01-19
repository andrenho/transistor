#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH

#include <cstddef>
#include <functional>
#include <string>

#include "engine/util/types.hh"
#include "ui/scene.hh"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class  ResourceManager;
class  Scene;
struct Component;

struct ComponentDefinition {
    enum class Type { SingleTile, IC_DIP, IC_Quad };

    std::string name;
    Type        type;
    bool        can_rotate;

    size_t      data_size = 0;


    std::function<void(Component& component)>                         on_click = [](Component&) {};
    std::function<std::vector<uintpin_t>(Component const& component)> input_pins = [](Component const&) { return std::vector<uintpin_t>{}; };
    std::function<void(Component& component)>                         simulate = [](Component&) {};

    std::function<void(std::optional<Component const*> opt_component, Scene& scene, int x, int y, Pen pen)>
                                                                      render = [](std::optional<Component const*>, Scene&, int, int, Pen) {};

    std::function<std::string(Component const& component)>            serialize_component = [](Component const&) { return ""; };
    std::function<void(Component& component, json const& content)>    unserialize_component = [](Component&, json const&) {};

    constexpr uintpin_t pin_count() const {
        if (type == Type::SingleTile)
            return 4;
        throw std::runtime_error("IC type not yet supported for `pin_count`.");
    }

    [[nodiscard]] json serialize(Component const& component) const {
        json r { { "name", name } };
        std::string value = serialize_component(component);
        if (value != "")
            r["value"] = value;
        return r;
    }

    // TODO - add IC fields (size, pins)
};

#endif //COMPONENTDEFINITION_HH
