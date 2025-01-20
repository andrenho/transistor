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

std::pair<Position, Position> Component::rect(Position const& component_pos) const
{
    if (def->type == ComponentDefinition::Type::SingleTile)
        return { component_pos, component_pos };

    if (def->type == ComponentDefinition::Type::IC_DIP) {
        auto h = (intpos_t) (def->pins.size() / 2);
        if (rotation == Direction::N || rotation == Direction::S)
            return { component_pos.add(-1, -1), component_pos.add(def->width, h) };
        else
            return { component_pos.add(-1, -1), component_pos.add(h, def->width) };
    }

    throw std::runtime_error("Not implemented yet.");  // TODO
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

    } else if (def->type == ComponentDefinition::Type::IC_DIP) {
        auto h = (intpos_t) (n_pins / 2);
        uintpin_t j = 0;
        switch (rotation) {
            case Direction::N:
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x - 1, component_pos.y + i } });
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + def->width, component_pos.y + i } });
                break;
            case Direction::E:
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y + def->width } });
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y - 1 } });
                break;
            case Direction::S:
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + def->width, component_pos.y + i } });
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x - 1, component_pos.y + i } });
                break;
            case Direction::W:
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y - 1 } });
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y + def->width } });
                break;
            case Direction::Center:
            default:
                throw std::runtime_error("Not applicable");
        }

    } else {
        throw std::runtime_error("`pin_positions` for IC not supported yet"); // TODO
    }

    return pin_positions;
}
