#ifndef COMPONENTDEFINITION_HH
#define COMPONENTDEFINITION_HH

#include <cstddef>
#include <functional>
#include <string>

#include "engine/util/types.hh"
#include "ui/scene.hh"

#include <nlohmann/json.hpp>

#include "engine/board/wire.hh"
#include "engine/geometry/position.hh"

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

    enum class Category {
        Basic, LogicGates, Digital, Memory, CPU,
    };

    std::string               name;
    Category                  category;
    std::string               tool_path = "";
    std::string               infobox {};

    Type                      type;
    bool                      can_rotate = true;
    std::vector<ComponentPin> pins;

    size_t                    data_size = 0;
    uint8_t                   width = 1;

    std::function<void()>                                          init = [](){};

    std::function<void(Component& component)>                      on_click = [](Component&) {};
    std::function<void(Component& component)>                      simulate = [](Component&) {};

    std::function<void(std::optional<Component const*> opt_component, Scene& scene, int x, int y, Pen pen)>
                                                                   render = [](std::optional<Component const*>, Scene&, int, int, Pen) {};

    std::function<std::string(Component const& component)>         serialize_component = [](Component const&) { return ""; };
    std::function<void(Component& component, json const& content)> unserialize_component = [](Component&, json const&) {};

    [[nodiscard]] json serialize(Component const& component) const;

    [[nodiscard]] std::pair<Position, Position> rect(Position const& component_pos, Direction dir) const;
    [[nodiscard]] std::vector<std::pair<uintpin_t, Position>> pin_positions(Position const& component_pos, Direction dir) const;

    void initialize();
    bool initialized = false;
};

#endif //COMPONENTDEFINITION_HH
