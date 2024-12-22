#include "compiler.hh"

namespace compiler {

Layout compile_to_layout(Board const& board)
{
    Layout layout;

    // create components
    for (auto& [pos, component]: board.components())
        for (auto const& [idx, pin_pos]: component.pin_positions(pos))
            layout.pins[pin_pos] = Pin { .component = &component, .pin_no = idx };

    // copy wires
    for (auto const& [pos, wire]: board.wires())
        layout.wires[pos] = wire;

    return layout;
}

Connections compile_to_connections(std::vector<Layout> const& layout)
{
    Connections connections;

    return connections;
}

}