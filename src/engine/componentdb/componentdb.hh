#ifndef COMPONENTDATABASE_HH
#define COMPONENTDATABASE_HH

#include <memory>
#include <string>
#include <unordered_map>

#include "engine/board/component.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ComponentDatabase {
public:
    ComponentDatabase();

    Component create_component(std::string const& name) const;

    std::optional<ComponentDefinition const*> component_def(std::string const& name) const;

    void add(ComponentDefinition const& def);
    void remove(std::string const& name);

    [[nodiscard]] std::unordered_map<std::string, std::unique_ptr<ComponentDefinition>> const& component_defs() const { return components_; }

    [[nodiscard]] json serialize() const;

private:
    std::unordered_map<std::string, std::unique_ptr<ComponentDefinition>> components_;

    void validate_component(ComponentDefinition const& def);
    static std::string reformat_infobox(std::string const& text);
};

#endif //COMPONENTDATABASE_HH
