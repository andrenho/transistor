#include "component.hh"

static constexpr Direction single_tile_next_pin(Direction dir)
{
    switch (dir) {
        case Direction::N: return Direction::W;
        case Direction::W: return Direction::S;
        case Direction::S: return Direction::E;
        case Direction::E: return Direction::N;
        case Direction::Center: return Direction::Center;
        default: throw std::runtime_error("Shouldn't happen");
    }
}

std::vector<std::pair<uintpin_t, Position>> Component::pin_positions(Position const& component_pos) const
{
    std::vector<std::pair<uintpin_t, Position>> pin_positions;

    uintpin_t n_pins = def->pins.size();
    if (def->type == ComponentDefinition::Type::SingleTile) {
        Direction dir = rotation;
        if (n_pins == 4) {
            for (uintpin_t i = 0; i < n_pins; ++i) {
                pin_positions.push_back({ i, { component_pos.board_id, component_pos.x, component_pos.y, dir } });
                dir = single_tile_next_pin(dir);
            }
        } else if (n_pins == 2) {
            pin_positions.push_back({ 0, { component_pos.board_id, component_pos.x, component_pos.y, dir } });
            dir = single_tile_next_pin(single_tile_next_pin(dir));
            pin_positions.push_back({ 1, { component_pos.board_id, component_pos.x, component_pos.y, dir } });
        } else if (n_pins == 1) {
            pin_positions.push_back({ 0, { component_pos.board_id, component_pos.x, component_pos.y, dir } });
        }

    } else {
        throw std::runtime_error("`pin_positions` for IC not supported yet"); // TODO
    }

    return pin_positions;
}
