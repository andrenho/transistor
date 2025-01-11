#ifndef COMPONENTDATABASE_HH
#define COMPONENTDATABASE_HH

#include <memory>
#include <string>
#include <unordered_map>

#include "engine/editor/component.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ComponentDatabase {
public:
    ComponentDatabase();

    Component create_component(std::string const& name) const;
    ComponentDefinition const& component_def(std::string const& name) const { return *components_.at(name).get(); }

    [[nodiscard]] json serialize() const;

private:
    std::unordered_map<std::string, std::unique_ptr<ComponentDefinition>> components_;
};

#endif //COMPONENTDATABASE_HH
