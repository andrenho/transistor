#include "sandbox.hh"

#include <algorithm>

#include "compiler.hh"
#include "engine/layout/layout.hh"

void Sandbox::reset()
{
    components_cache_.clear();

    // compile boards and generate a layout cache
    std::vector<Layout> layouts;
    for (auto& board: editor_.boards()) {
        layouts.push_back(compiler::compile_to_layout(board));

        // extend component cache with all components on board
        components_cache_.reserve(components_cache_.size() + board.components().size());
        for (auto& [_, component]: board.components())
            components_cache_.push_back(&component);
    }

    connections_cache_ = compiler::compile_to_connections(layouts);
}

void Sandbox::simulate()
{
    wire_value_cache_.clear();

    for (Connection& connection: connections_cache_) {

        // set pin input values
        for (Pin const& pin: connection.pins) {
            pin.component->pins[pin.pin_no] = connection.value;
        }

        // simulate components
        for (Component* component: components_cache_)
            component->def->simulate(*component);

        // find value for connection
        bus_data_t value = 0;
        for (Pin const& pin: connection.pins)
            if (!pin.input)
                value |= pin.component->pins[pin.pin_no];
        connection.value = value;

        // add wire cache
        for (Position const& pos: connection.wires)
            wire_value_cache_.emplace(pos, value);
    }
}

uint8_t Sandbox::wire_value(Position const& pos) const
{
    auto it = wire_value_cache_.find(pos);
    if (it != wire_value_cache_.end())
        return it->second;
    return 0;
}

json Sandbox::serialize() const
{
    json content;
    return content;
}