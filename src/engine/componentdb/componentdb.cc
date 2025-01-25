#include "componentdb.hh"

#include <regex>

#include "components.hh"
#include "util/exceptions.hh"

ComponentDatabase::ComponentDatabase()
{
    static const ComponentDefinition native_components[] = {
        component::button(), component::led(), component::vcc(), component::npn(), component::pnp(),
        component::or_2i(),
    };

    for (auto const& c: native_components)
        add(c);
}

Component ComponentDatabase::create_component(std::string const& name) const
{
    ComponentDefinition* def = components_.at(name).get();
    def->initialize();
    Component component {
        .def = def,
        .pins = std::make_unique<uint8_t[]>(def->pins.size()),
    };
    if (def->data_size > 0)
        component.data = std::make_unique<uint8_t[]>(def->data_size);
    return component;
}

void ComponentDatabase::add(ComponentDefinition const& def)
{
    validate_component(def);
    auto pdef = std::make_unique<ComponentDefinition>(def);
    pdef->infobox = reformat_infobox(pdef->infobox);
    components_[def.name] = std::move(pdef);
}

void ComponentDatabase::remove(std::string const& name)
{
    components_.erase(name);
}

std::optional<ComponentDefinition const*> ComponentDatabase::component_def(std::string const& name) const
{
    auto it = components_.find(name);
    if (it == components_.end())
        return {};
    return it->second.get();
}

void ComponentDatabase::validate_component(ComponentDefinition const& def)
{
    if (def.name.empty())
        throw ComponentValidationFailed("Component doesn't have a valid name");

    size_t n_pins = def.pins.size();
    if (n_pins == 0)
        throw ComponentValidationFailed("Component can't have zero pins");

    if (def.type == ComponentDefinition::Type::SingleTile) {
        if (n_pins != 1 && n_pins != 2 && n_pins != 4)
            throw ComponentValidationFailed("Single-tile component must have 1, 2 or 4 pins.");
    } else if (def.type == ComponentDefinition::Type::IC_DIP) {
        if (n_pins % 2 != 0)
            throw ComponentValidationFailed("IC_DIP component pin count must be divisible per 2");
    } else if (def.type == ComponentDefinition::Type::IC_Quad) {
        if (n_pins % 4 != 0)
            throw ComponentValidationFailed("IC_Quad component pin count must be divisible per 4");
    }

    if (def.width < 1)
        throw ComponentValidationFailed("Width can't be < 1");
    if (def.width > 1 && def.type != ComponentDefinition::Type::IC_DIP)
        throw ComponentValidationFailed("Widths are only applicable to IC_DIP.");
}

json ComponentDatabase::serialize() const
{
    return json::object();  // TODO
}

std::string ComponentDatabase::reformat_infobox(std::string const& text)
{
    static const std::regex leading_whitespace(R"(^\s+)");     // matches leading whitespace
    static const std::regex trailing_whitespace(R"(\s+$)");    // matches trailing whitespace
    static const std::regex multiple_spaces(R"([ \t]{2,})");      // matches 2 or more whitespace characters
    static const std::regex enter_and_space(R"(\n[ \t])");

    std::string result = text;
    result = std::regex_replace(result, leading_whitespace, "");
    result = std::regex_replace(result, multiple_spaces, " ");
    result = std::regex_replace(result, enter_and_space, "\n");
    result = std::regex_replace(result, trailing_whitespace, "");
    return result;
}
