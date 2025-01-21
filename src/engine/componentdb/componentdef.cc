#include "componentdef.hh"

#include "engine/board/component.hh"

json ComponentDefinition::serialize(Component const& component) const
{
    json r {
        { "name", name },
        { "dir", std::string(1, (char) component.rotation) },
    };
    std::string value = serialize_component(component);
    if (!value.empty())
        r["value"] = value;
    return r;
}

std::pair<Position, Position> ComponentDefinition::rect(Position const& component_pos, Direction rotation) const
{
    if (type == ComponentDefinition::Type::SingleTile)
        return { component_pos, component_pos };

    if (type == ComponentDefinition::Type::IC_DIP) {
        auto h = (intpos_t) (pins.size() / 2);
        if (rotation == Direction::N || rotation == Direction::S)
            return { component_pos.add(-1, -1), component_pos.add(width, h) };
        else
            return { component_pos.add(-1, -1), component_pos.add(h, width) };
    }

    if (type == ComponentDefinition::Type::IC_Quad) {
        auto h = (intpos_t) (pins.size() / 4);
        return { component_pos.add(-1, -1), component_pos.add(h, h) };
    }

    throw std::runtime_error("Not applicable.");

}

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

std::vector<std::pair<uintpin_t, Position>> ComponentDefinition::pin_positions(Position const& component_pos, Direction rotation) const
{
    std::vector<std::pair<uintpin_t, Position>> pin_positions;

    uintpin_t n_pins = pins.size();
    if (type == ComponentDefinition::Type::SingleTile) {
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

    } else if (type == ComponentDefinition::Type::IC_DIP) {
        auto h = (intpos_t) (n_pins / 2);
        uintpin_t j = 0;
        switch (rotation) {
            case Direction::N:
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x - 1, component_pos.y + i } });
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + width, component_pos.y + i } });
                break;
            case Direction::E:
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y + width } });
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y - 1 } });
                break;
            case Direction::S:
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + width, component_pos.y + i } });
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x - 1, component_pos.y + i } });
                break;
            case Direction::W:
                for (int i = (h-1); i >= 0; --i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y - 1 } });
                for (int i = 0; i < h; ++i)
                    pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y + width } });
                break;
            case Direction::Center:
            default:
                throw std::runtime_error("Not applicable");
        }

    } else if (type == ComponentDefinition::Type::IC_Quad) {
        auto h = (intpos_t) (n_pins / 4);
        uintpin_t j = 0;
        auto add_w = [&]() {
            for (int i = 0; i < h; ++i)
                pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x - 1, component_pos.y + i } });
        };
        auto add_s = [&]() {
            for (int i = 0; i < h; ++i)
                pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y + h } });
        };
        auto add_e = [&]() {
            for (int i = (h-1); i >= 0; --i)
                pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + h, component_pos.y + i } });
        };
        auto add_n = [&]() {
            for (int i = (h-1); i >= 0; --i)
                pin_positions.push_back({ j++, { component_pos.board_id, component_pos.x + i, component_pos.y - 1 } });
        };
        switch (rotation) {
            case Direction::N: add_w(), add_s(), add_e(), add_n(); break;
            case Direction::E: add_n(), add_w(), add_s(), add_e(); break;
            case Direction::S: add_e(), add_n(), add_w(), add_s(); break;
            case Direction::W: add_s(), add_e(), add_n(), add_w(); break;
            case Direction::Center:
            default:
                throw std::runtime_error("Not applicable");
        }

    } else {
        throw std::runtime_error("Not applicable");
    }

    return pin_positions;

}