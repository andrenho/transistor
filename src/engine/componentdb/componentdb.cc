#include "componentdb.hh"

#include "components.hh"

static const ComponentDefinition native_components[] = {
    component::button(), component::led()
};

ComponentDatabase::ComponentDatabase()
{
    for (auto const& c: native_components)
        components_[c.name] = std::make_unique<ComponentDefinition>(c);
}

Component ComponentDatabase::create_component(std::string const& name) const
{
    return { .def = components_.at(name).get() };
}
