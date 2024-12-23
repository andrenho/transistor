#include "board.hh"

#include "engine/sandbox/sandbox.hh"

Component* Board::add_component(std::string const& component_name, intpos_t x, intpos_t y)
{
    // TODO - check - is there a component here already?
    auto it = components_.emplace(Position { x, y, Direction::Center }, component_db_.create_component(component_name));
    sandbox_.on_update();
    return &it.first->second;
}

void Board::draw_wire(Wire::Width width, Wire::Layer layer, intpos_t x0, intpos_t y0, intpos_t x1, intpos_t y1, Orientation orientation)
{
    wire_management_.start_drawing({ x0, y0 }, width, layer);
    for (auto const& [pos, wire] : wire_management_.stop_drawing({ x1, y1 }))
        wires_[pos] = wire;
    sandbox_.on_update();
}
