#include "componentdb.hh"

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
    ComponentDefinition const* def = components_.at(name).get();
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
    if (def.name.empty())
        throw ComponentValidationFailed("Component doesn't have a valid name");

    size_t n_pins = def.pins.size();
    if (n_pins == 0)
        throw ComponentValidationFailed("Component can't have zero pins");

    if (def.type == ComponentDefinition::Type::SingleTile) {
        if (n_pins != 1 && n_pins != 2 && n_pins != 4)
            throw ComponentValidationFailed("Single-tile component must have 1, 2 or 4 pins.");
    }

    components_[def.name] = std::make_unique<ComponentDefinition>(def);
}

void ComponentDatabase::remove(std::string const& name)
{
    components_.erase(name);
}

json ComponentDatabase::serialize() const
{
    return json::object();  // TODO
}
