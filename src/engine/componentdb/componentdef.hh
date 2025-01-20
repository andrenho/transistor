#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH

#include <cstddef>
#include <functional>
#include <string>

#include "engine/util/types.hh"
#include "ui/scene.hh"

#include <nlohmann/json.hpp>

#include "engine/board/wire.hh"

using json = nlohmann::json;

class  ResourceManager;
class  Scene;
struct Component;

enum class InputPinType { Input, Output };

struct ComponentDefinition {
    enum class Type { SingleTile, IC_DIP, IC_Quad };

    struct ComponentPin {
        std::string  name;
        InputPinType type;
        Wire::Width  wire_width = Wire::Width::W1;

        ComponentPin(std::string const& name_, InputPinType type_) : name(name_), type(type_) {}
        ComponentPin(std::string const& name_, InputPinType type_, Wire::Width wire_width_) : name(name_), type(type_), wire_width(wire_width_) {}
    };

    std::string               name;
    Type                      type;
    bool                      can_rotate = true;
    std::vector<ComponentPin> pins;

    size_t                    data_size = 0;

    std::function<void(Component& component)>                      on_click = [](Component&) {};
    std::function<void(Component& component)>                      simulate = [](Component&) {};

    std::function<void(std::optional<Component const*> opt_component, Scene& scene, int x, int y, Pen pen)>
                                                                   render = [](std::optional<Component const*>, Scene&, int, int, Pen) {};

    std::function<std::string(Component const& component)>         serialize_component = [](Component const&) { return ""; };
    std::function<void(Component& component, json const& content)> unserialize_component = [](Component&, json const&) {};

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
