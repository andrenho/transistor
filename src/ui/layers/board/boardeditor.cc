#include "boardeditor.hh"

#include <iostream>

#include "battery/embed.hpp"
#include "circuit_atlas.hh"

BoardEditor::BoardEditor(ResourceManager& resource_manager, Board& board)
    : Layer(0, 0, (board.w() + 4) * TILE_SIZE, (board.w() + 4) * TILE_SIZE), board_(board)
{
    zoom_ = 2.f;
}

//------------//
//            //
//   EVENTS   //
//            //
//------------//

auto to_pos = [](Board const& board, int x, int y) -> Position { return { board.id(), (intpos_t) (x / TILE_SIZE - 2), (intpos_t) (y / TILE_SIZE - 2) }; };

void BoardEditor::on_mouse_press(int x, int y, uint8_t button, bool dbl_click, Events& events)
{
    auto pos = to_pos(board_, x, y);

    if (button == 1) {

        // check for component click
        auto it = board_.components().find(pos);
        if (it != board_.components().end())
            it->second.on_click();

    } else if (button == 3) {
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
    auto pos = to_pos(board_, x, y);

    if (drawing_wire_)
        board_.continue_placing_wire(pos.x, pos.y);
    if (erasing_wire_)
        board_.clear_tile(pos.x, pos.y);
}

void BoardEditor::on_key_press(uint32_t key, int x, int y, Events& events)
{
    auto pos = to_pos(board_, x, y);

    switch (key) {
        case 'w':
            drawing_wire_ = true;
            board_.start_placing_wire(Wire::Width::W1, Wire::Layer::Top, pos.x, pos.y);
            break;
        case 'b':
            board_.add_component("button", pos.x, pos.y);
            break;
        case 'l':
            board_.add_component("led", pos.x, pos.y);
            break;
        case 'v':
            board_.add_component("vcc", pos.x, pos.y);
            break;
        case 'x':
            board_.clear_tile(pos.x, pos.y);
            erasing_wire_ = true;
            events.emplace_back(event::SetMouseCursor { event::SetMouseCursor::Delete });
            break;
        default: break;
    }
}

void BoardEditor::on_key_release(uint32_t key, int x, int y, Events& events)
{
    auto pos = to_pos(board_, x, y);

    if (key == 'w') {
        drawing_wire_ = false;
        board_.finish_placing_wire(pos.x, pos.y);
    } else if (key == 'x') {
        erasing_wire_ = false;
        events.emplace_back(event::SetMouseCursor { event::SetMouseCursor::Normal });
    }
}

//---------------//
//               //
//   RENDERING   //
//               //
//---------------//

void BoardEditor::draw(Scene& scene, CSprite sprite, int x, int y, Pen const& pen) const
{
    scene.add(sprite, (x + 2) * TILE_SIZE, (y + 2) * TILE_SIZE, pen);
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
        auto it = board_.wires().find({ board_.id(), x, y, dir });
        if (it != board_.wires().end())
            render_wire(scene, it->first, it->second, false);

        // draw temporary wire
        auto temp = board_.temporary_wire();
        it = temp.find({ board_.id(), x, y, dir });
        if (it != temp.end())
            render_wire(scene, it->first, it->second, true);

        auto itc = board_.components().find({ board_.id(), x, y });
        if (itc != board_.components().end())
            render_component(scene, itc->first, itc->second);
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

    auto it = wire_sprites.find({ wire, pos.dir, board_.wire_value(pos) });
    if (it == wire_sprites.end())
        throw std::runtime_error("Wire configuration not found");
    draw(scene, it->second, pos.x, pos.y, { .semitransparent = semitransparent });
}

void BoardEditor::render_component(Scene& scene, Position const& pos, Component const& component) const
{
    component.def->render(component, scene, (pos.x + 2) * TILE_SIZE, (pos.y + 2) * TILE_SIZE);
}
