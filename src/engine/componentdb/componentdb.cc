#include "componentdb.hh"

#include "components.hh"

ComponentDatabase::ComponentDatabase()
{
    /*
    res().add_tiles("icons", {
        { "shadow_rect", 5, 4 },
        { "shadow_square", 6, 4 },
        { "shadow_circle", 7, 4 },
    }, 16);
    */

    static const ComponentDefinition native_components[] = {
        component::button(), component::led(), component::vcc(), component::npn(), component::pnp(),
    };

    for (auto const& c: native_components)
        components_[c.name] = std::make_unique<ComponentDefinition>(c);
}

Component ComponentDatabase::create_component(std::string const& name) const
{
    ComponentDefinition const* def = components_.at(name).get();
    Component component {
        .def = def,
        .pins = std::make_unique<uint8_t[]>(def->pin_count()),
    };
    if (def->data_size > 0)
        component.data = std::make_unique<uint8_t[]>(def->data_size);
    return component;
}

json ComponentDatabase::serialize() const
{
    return json::object();  // TODO
}
