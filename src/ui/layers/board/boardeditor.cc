#include "boardeditor.hh"

#include <iostream>

#include "battery/embed.hpp"
#include "circuit_atlas.hh"

BoardEditor::BoardEditor(ResourceManager& resource_manager, Board& board)
    : Layer(0, 0, (board.w() + 4) * TILE_SIZE, (board.w() + 4) * TILE_SIZE), board_(board)
{
    Resource circuit = resource_manager.from_image(b::embed<"resources/images/circuit.png">().vec());
    icons_ = resource_manager.from_atlas(circuit, circuit_coordinates, TILE_SIZE);
    zoom_ = 2.f;
}

//------------//
//            //
//   EVENTS   //
//            //
//------------//

auto to_pos = [](int x, int y) -> Position { return { (intpos_t) (x / TILE_SIZE - 2), (intpos_t) (y / TILE_SIZE - 2) }; };

void BoardEditor::on_mouse_press(int x, int y, uint8_t button, bool dbl_click, Events& events)
{
    if (button == 3) {
        events.emplace_back(event::StartDragging { this });
    }
}

void BoardEditor::on_mouse_release(int x, int y, uint8_t button, Events& events)
{
    if (button == 3) {
        events.emplace_back(event::StopDragging {});
    }
}

void BoardEditor::on_mouse_move(int x, int y, int rx, int ry, Events& events)
{
    auto pos = to_pos(x, y);

    if (drawing_wire_)
        board_.continue_placing_wire(pos.x, pos.y);
}

void BoardEditor::on_key_press(uint32_t key, int x, int y, Events& events)
{
    auto pos = to_pos(x, y);

    if (key == 'w') {
        drawing_wire_ = true;
        board_.start_placing_wire(Wire::Width::W1, Wire::Layer::Top, pos.x, pos.y);
    }
}

void BoardEditor::on_key_release(uint32_t key, int x, int y, Events& events)
{
    auto pos = to_pos(x, y);

    if (key == 'w') {
        drawing_wire_ = false;
        board_.finish_placing_wire(pos.x, pos.y);
    }
}

//---------------//
//               //
//   RENDERING   //
//               //
//---------------//

void BoardEditor::draw(Scene& scene, CSprite sprite, int x, int y, Pen const& pen) const
{
    scene.add(this, icons_.at(static_cast<size_t>(sprite)), (x + 2) * TILE_SIZE, (y + 2) * TILE_SIZE, pen);
}

void BoardEditor::render(Scene& scene)
{
    render_border(scene);
    for (intpos_t x = 0; x < board_.w(); ++x)
        for (intpos_t y = 0; y < board_.h(); ++y)
            render_tile(scene, x, y);
}

void BoardEditor::render_border(Scene& scene) const
{
    draw(scene, CSprite::BoardTopLeft, -2, -2);
    draw(scene, CSprite::BoardTopRight, board_.w(), -2);
    draw(scene, CSprite::BoardBottomLeft, -2, board_.h());
    draw(scene, CSprite::BoardBottomRight, board_.w(), board_.h());

    for (ssize_t x = 0; x < board_.w(); ++x) {
        draw(scene, CSprite::BoardTop, x, -2);
        draw(scene, CSprite::BoardBottom, x, board_.h());
    }

    for (ssize_t y = 0; y < board_.h(); ++y) {
        draw(scene, CSprite::BoardLeft, -2, y);
        draw(scene, CSprite::BoardRight, board_.w(), y);
    }
}

void BoardEditor::render_tile(Scene& scene, intpos_t x, intpos_t y) const
{
    draw(scene, CSprite::Tile, x, y);

    for (Direction const& dir: DIRECTIONS) {
        // draw wire
        auto it = board_.wires().find({ x, y, dir });
        if (it != board_.wires().end())
            render_wire(scene, it->first, it->second, false);

        // draw temporary wire
        auto temp = board_.temporary_wire();
        it = temp.find({ x, y, dir });
        if (it != temp.end())
            render_wire(scene, it->first, it->second, true);
    }
}

// why is that not included by default in C++ ??
template <>
struct std::hash<std::tuple<Wire, Direction, bool>> {
    std::size_t operator()(std::tuple<Wire, Direction, bool> const& t) const noexcept {
        return (std::hash<Wire>()(std::get<0>(t)) << 8) ^ std::hash<Direction>()(std::get<1>(t)) << std::get<2>(t);
    }
};

void BoardEditor::render_wire(Scene& scene, Position const& pos, Wire const& wire, bool semitransparent) const
{
    static const std::unordered_map<std::tuple<Wire, Direction, bool>, CSprite> wire_sprites {
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::N, true }, CSprite::WireTopOnNorth_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::S, true }, CSprite::WireTopOnSouth_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::W, true }, CSprite::WireTopOnWest_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::E, true }, CSprite::WireTopOnEast_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::N, false }, CSprite::WireTopOffNorth_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::S, false }, CSprite::WireTopOffSouth_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::W, false }, CSprite::WireTopOffWest_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::E, false }, CSprite::WireTopOffEast_1 },
    };

    auto it = wire_sprites.find({ wire, pos.dir, false });  // TODO: use actual value
    if (it == wire_sprites.end())
        throw std::runtime_error("Wire configuration not found");
    draw(scene, it->second, pos.x, pos.y, { .semitransparent = semitransparent });
}
