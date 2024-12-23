#include "compiler.hh"

#include <unordered_set>

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

std::vector<std::unordered_set<Position>> find_connected_wires(std::unordered_set<Position> wires)
{
    auto find_connected_group = [&wires](Position const& start) -> std::unordered_set<Position> {
        return {};
    };

    if (wires.empty())
        return {};

    std::vector<std::unordered_set<Position>> groups;

    while (!wires.empty())
        groups.push_back(find_connected_group(*wires.begin()));

    return groups;
}

Connections compile_to_connections(std::vector<Layout> const& layout)
{
    Connections connections;

    return connections;
}

}