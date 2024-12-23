#include "component.hh"

std::vector<std::pair<uintpin_t, Position>> Component::pin_positions(Position const& component_pos) const
{
    std::vector<std::pair<uintpin_t, Position>> pin_positions;

    if (def->type == ComponentDefinition::Type::SingleTile) {
        Direction dir = rotation;
        for (size_t i = 0; i < 4; ++i) {
            pin_positions.push_back({ i, { component_pos.x, component_pos.y, dir } });
            dir = rotate(dir);
        }
    } else if (def->type == ComponentDefinition::Type::IC) {
        throw std::runtime_error("`pin_positions` for IC not supported yet"); // TODO
    }

    return pin_positions;
}
