#include "board.hh"

void Board::add_component(std::string const& component_name, intpos_t x, intpos_t y)
{
    // TODO - check - is there a component here already?
    components_[{ x, y, Direction::Center }] = component_db_.create_component(component_name);
}
