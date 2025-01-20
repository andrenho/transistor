#include "compiler.hh"

#include <iostream>
#include <unordered_set>

namespace compiler {

Layout compile_to_layout(Board const& board)
{
    Layout layout;

    // create components
    for (auto& [pos, component]: board.components()) {

        // find input pins
        std::vector<uintpin_t> input_pins;
        for (size_t i = 0; i < component.def->pins.size(); ++i)
            if (component.def->pins.at(i).type == InputPinType::Input)
                input_pins.push_back(i);

        // create component
        for (auto const& [idx, pin_pos]: component.pin_positions(pos))
            layout.pins[pin_pos] = Pin { .component = &component, .pin_no = idx, .input = r::contains(input_pins, idx) };
    }

    // copy wires
    for (auto const& [pos, wire]: board.wires())
        layout.wires.emplace(pos, wire);

    return layout;
}

std::vector<std::unordered_set<Position>> find_connected_wires(std::unordered_set<Position> wires, std::unordered_set<Position> const& single_tile_component_pins)
{
    auto find_connected_group = [&wires, &single_tile_component_pins](Position const& start) -> std::unordered_set<Position> {
        std::unordered_set<Position> result;
        std::unordered_set<Position> to_visit;

        to_visit.insert(start);

        while (!to_visit.empty()) {
            Position const& visiting = *to_visit.begin();

            if (wires.contains(visiting)) {
                // add to result list, and remove from the visited
                result.insert(visiting);
                wires.erase(visiting);

                // add neighbours
                bool has_single_tile_component_pin = r::contains(single_tile_component_pins, Position { visiting.board_id, visiting.x, visiting.y });
                for (Position const& neighbour: visiting.neighbours(has_single_tile_component_pin))
                    to_visit.insert(neighbour);
            }

            to_visit.erase(visiting);
        }

        return result;
    };

    if (wires.empty())
        return {};

    std::vector<std::unordered_set<Position>> groups;

    while (!wires.empty())
        groups.push_back(find_connected_group(*wires.begin()));

    return groups;
}

Connections compile_to_connections(std::vector<Layout> const& layouts)
{
    Connections connections;

    auto create_connection = [](Layout const& layout, std::unordered_set<Position> const& positions) -> Connection {
        Connection conn;
        conn.wires = positions;
        for (auto const& [pos, pin]: layout.pins) {
            if (conn.wires.contains(pos))
                conn.pins.insert(pin);
            if (conn.wires.contains({ pos.board_id, pos.x, pos.y, Direction::Center }))
                conn.pins.insert(pin);
        }
        return conn;
    };

    // for each layout...
    for (auto const& layout: layouts) {

        // ...find single-tile component pins positions, ...
        std::unordered_set<Position> single_tile_component_pins;
        for (auto const& p: layout.pins)
            if (p.second.component->def->type == ComponentDefinition::Type::SingleTile)
                single_tile_component_pins.emplace(p.first.board_id, p.first.x, p.first.y);

        // for each layer/width, ...
        for (auto const& layer: Wire::ALL_LAYERS) {
            for (auto const& width: Wire::ALL_WIDTHS) {

                // ... create list of wires
                std::unordered_set<Position> wires;
                for (auto const& [pos, wire]: layout.wires) {
                    if (wire.width == width && wire.layer == layer)
                        wires.insert(pos);
                }

                // ... find groups
                for (auto const& group : find_connected_wires(wires, single_tile_component_pins))
                    connections.push_back(create_connection(layout, group));
            }
        }
    }

    return connections;
}

}