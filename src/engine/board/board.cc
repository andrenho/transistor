#include "board.hh"

#include <iostream>

#include "engine/geometry/rect.hh"
#include "engine/sandbox/sandbox.hh"

size_t Board::board_counter_ = 0;

Board::Board(intpos_t w, intpos_t h, ComponentDatabase const& component_db)
    : id_(board_counter_++), w_(w), h_(h), component_db_(component_db)
{
}

Board::Board(json const& content, ComponentDatabase const& component_db)
    : id_(content.at("id")), w_(content.at("w")), h_(content.at("h")), component_db_(component_db)
{
    if (board_counter_ <= id_)
        board_counter_ = id_ + 1;

    for (auto const& jwire: content.at("wires").items())
        wires_.emplace(Position(jwire.key()), Wire(jwire.value()));

    for (auto const& jcomp: content.at("components").items()) {
        Position pos(jcomp.key());
        std::string dir_s = jcomp.value().at("dir");
        auto o_component = add_component(jcomp.value().at("name"), pos.x, pos.y, (Direction) dir_s[0]);
        if (o_component && jcomp.value().contains("value"))
            (*o_component)->def->unserialize_component(**o_component, jcomp.value().at("value"));
    }

    // recompile_();
}

std::optional<Component*> Board::add_component(std::string const& component_name, intpos_t x, intpos_t y, Direction dir)
{
    ComponentDefinition const& def = component_db_.component_def(component_name);
    auto [r1, r2] = def.rect({ id_, x, y }, dir);

    for (auto const& [pos, component]: components_) {
        auto [c1, c2] = component.rect(pos);
        if (overlap(r1, r2, c1, c2))
            return {};
    }

    if (def.type != ComponentDefinition::Type::SingleTile)
        remove_wires_for_ic(r1, r2);

    auto it = components_.emplace(Position { this, x, y, Direction::Center }, component_db_.create_component(component_name));
    Component* component = &it.first->second;
    component->rotation = dir;
    return &it.first->second;
}

void Board::remove_wires_for_ic(Position const& r1, Position const& r2)
{
    // remove sides
    for (intpos_t y = r1.y; y <= r2.y; ++y) {  // left
        wires_.erase({ id_, r1.x, y, Direction::N });
        wires_.erase({ id_, r1.x, y, Direction::S });
        wires_.erase({ id_, r1.x, y, Direction::E });
    }
    for (intpos_t y = r1.y; y <= r2.y; ++y) {  // right
        wires_.erase({ id_, r2.x, y, Direction::N });
        wires_.erase({ id_, r2.x, y, Direction::S });
        wires_.erase({ id_, r2.x, y, Direction::W });
    }
    for (intpos_t x = r1.x; x <= r2.x; ++x) {  // top
        wires_.erase({ id_, x, r1.y, Direction::S });
        wires_.erase({ id_, x, r1.y, Direction::W });
        wires_.erase({ id_, x, r1.y, Direction::E });
    }
    for (intpos_t x = r1.x; x <= r2.x; ++x) {  // bottom
        wires_.erase({ id_, x, r2.y, Direction::N });
        wires_.erase({ id_, x, r2.y, Direction::W });
        wires_.erase({ id_, x, r2.y, Direction::E });
    }

    // remove center
    for (intpos_t x = r1.x + 1; x < r2.x; ++x) {
        for (intpos_t y = r1.y + 1; y < r2.y; ++y) {
            wires_.erase({ id_, x, y, Direction::N });
            wires_.erase({ id_, x, y, Direction::S });
            wires_.erase({ id_, x, y, Direction::W });
            wires_.erase({ id_, x, y, Direction::E });
        }
    }
}

void Board::draw_wire(Wire::Width width, Wire::Layer layer, intpos_t x0, intpos_t y0, intpos_t x1, intpos_t y1, Orientation orientation)
{
    wire_management_.start_drawing({ this, x0, y0 }, width, layer);
    for (auto const& [pos, wire] : wire_management_.stop_drawing({ this, x1, y1 }))
        wires_.emplace(pos, wire);
}

void Board::merge_wires(std::map<Position, Wire> const& wires)
{
    for (auto const& [pos, wire]: wires)
        wires_.emplace(pos, wire);
}

void Board::clear_tile(intpos_t x, intpos_t y)
{
    wires_.erase({ this, x, y, Direction::Center });
    for (Direction dir: DIRECTIONS)
        wires_.erase({ this, x, y, dir });
    components_.erase({ this, x, y });
}

void Board::clear()
{
    wires_.clear();
    components_.clear();
}

void Board::rotate_component(intpos_t x, intpos_t y)
{
    auto it = components_.find({ id_, x, y });
    if (it != components_.end()) {
        Component& component = it->second;
        if (component.def->type == ComponentDefinition::Type::SingleTile) {
            if (component.def->can_rotate)
                component.rotation = dir_rotate_component(component.rotation);
        } else {
            throw std::runtime_error("IC rotation not supported yet.");  // TODO - implement for ICs
        }
    }
}

json Board::serialize() const
{
    json jwires = json::object();
    for (auto const& [pos, wire] : wires_)
        jwires[std::to_string(pos)] = std::to_string(wire);

    json jcomponents = json::object();
    for (auto const& [pos, component]: components_)
        jcomponents[std::to_string(pos)] = component.def->serialize(component);

    return {
        { "id", id_ },
        { "w", w_ },
        { "h", h_ },
        { "wires", jwires },
        { "components", jcomponents },
    };
}