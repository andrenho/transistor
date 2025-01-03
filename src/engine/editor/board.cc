#include "board.hh"

#include "engine/sandbox/sandbox.hh"

size_t Board::board_counter_ = 0;

Component* Board::add_component(std::string const& component_name, intpos_t x, intpos_t y)
{
    // TODO - check - is there a component here already?
    auto it = components_.emplace(Position { this, x, y, Direction::Center }, component_db_.create_component(component_name));
    sandbox_.reset();
    return &it.first->second;
}

void Board::draw_wire(Wire::Width width, Wire::Layer layer, intpos_t x0, intpos_t y0, intpos_t x1, intpos_t y1, Orientation orientation)
{
    wire_management_.start_drawing({ this, x0, y0 }, width, layer);
    for (auto const& [pos, wire] : wire_management_.stop_drawing({ this, x1, y1 }))
        wires_[pos] = wire;
    sandbox_.reset();
}

void Board::merge_wires(std::map<Position, Wire> const& wires)
{
    for (auto const& [pos, wire]: wires)
        wires_[pos] = wire;
    sandbox_.reset();
}

void Board::clear_tile(intpos_t x, intpos_t y)
{
    wires_.erase({ this, x, y, Direction::Center });
    for (Direction dir: DIRECTIONS)
        wires_.erase({ this, x, y, dir });
    components_.erase({ this, x, y });
    sandbox_.reset();
}

bus_data_t Board::wire_value(Position const& pos) const
{
    return sandbox_.wire_value(pos);
}
