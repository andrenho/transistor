#include "boardeditor.hh"

#include <iostream>

#include "battery/embed.hpp"
#include "res/resourcemanager.hh"
#include "ui/ui.hh"

constexpr int TILE_SIZE = 16;

BoardEditor::BoardEditor(size_t board_id)
    : DeviceEditor(0, 0, (game().board(board_id).w() + 4) * TILE_SIZE, (game().board(board_id).w() + 4) * TILE_SIZE),
      board_id_(board_id)
{
    zoom_ = 2.f;
}

void BoardEditor::load_icons()
{
    res().add_tiles("icons", {
        { &tile, 2, 2 },
        { &board_top_left, 0, 0, 2, 2 },
        { &board_top, 2, 0, 1, 2 },
        { &board_top_right, 3, 0, 2, 2 },
        { &board_left, 0, 2, 2, 1 },
        { &board_right, 3, 2, 2, 1 },
        { &board_bottom_left, 0, 3, 2, 2 },
        { &board_bottom, 2, 3, 1, 2 },
       { &board_bottom_right, 3, 3, 2, 2 },
        { &wire_top_on_north_1, 0, 7 },
        { &wire_top_on_east_1, 1, 7 },
        { &wire_top_on_west_1, 2, 7 },
        { &wire_top_on_south_1, 3, 7 },
        { &wire_top_off_north_1, 0, 5 },
        { &wire_top_off_east_1, 1, 5 },
        { &wire_top_off_west_1, 2, 5 },
        { &wire_top_off_south_1, 3, 5 },
    }, TILE_SIZE);
}

//------------//
//            //
//   EVENTS   //
//            //
//------------//

static Position to_pos(Board const& board, int x, int y)
{
    return { board.id(), (intpos_t) (x / TILE_SIZE - 2), (intpos_t) (y / TILE_SIZE - 2) };
};

static Position to_pos(size_t board_id, int x, int y)
{
    return { board_id, (intpos_t) (x / TILE_SIZE - 2), (intpos_t) (y / TILE_SIZE - 2) };
};


void BoardEditor::on_mouse_press(int x, int y, uint8_t button, bool dbl_click)
{
    Board const& board = game().board(board_id_);
    auto pos = to_pos(board, x, y);

    if (button == 1) {
        // check for component click
        auto it = board.components().find(pos);
        if (it != board.components().end())
            game() << G::ComponentClick { &it->second };

    } else if (button == 2) {

        start_erasing(pos);

    } else if (button == 3) {
        ui() << U::StartDragginDevice { this };
    }
}

void BoardEditor::on_mouse_release(int x, int y, uint8_t button)
{
    if (button == 2) {
        stop_erasing();
    } else if (button == 3) {
        ui() << U::StopDraggingDevice {};
    }
}

void BoardEditor::on_mouse_move(int x, int y, int rx, int ry)
{
    auto pos = to_pos(board_id_, x, y);

    if (drawing_wire_)
        game() << G::ContinuePlacingWire { pos };
    if (erasing_)
        game() << G::ClearTile { pos };
}

void BoardEditor::on_key_press(uint32_t key, int x, int y)
{
    auto pos = to_pos(board_id_, x, y);

    switch (key) {
        case 'w':
            drawing_wire_ = true;
            game() << G::StartPlacingWire { Wire::Width::W1, Wire::Layer::Top, pos };
            break;
        case 'b':
            game() << G::AddComponent { "button", pos };
            break;
        case 'l':
            game() << G::AddComponent { "led", pos };
            break;
        case 'v':
            game() << G::AddComponent { "vcc", pos };
            break;
        case 'n':
            game() << G::AddComponent { "npn", pos };
            break;
        case 'p':
            game() << G::AddComponent { "pnp", pos };
            break;
        case 'r':
            game() << G::RotateComponent { pos };
            break;
        case 'x':
            start_erasing(pos);
            break;
        default: break;
    }
}

void BoardEditor::on_key_release(uint32_t key, int x, int y)
{
    auto pos = to_pos(board_id_, x, y);

    if (key == 'w') {
        drawing_wire_ = false;
        game() << G::FinishPlacingWire { pos };
    } else if (key == 'x') {
        stop_erasing();
    }
}

void BoardEditor::start_erasing(Position const& pos)
{
    game() << G::ClearTile { pos };
    erasing_ = true;
    ui() << U::SetMouseCursor { U::SetMouseCursor::Delete };
}

void BoardEditor::stop_erasing()
{
    erasing_ = false;
    ui() << U::SetMouseCursor { U::SetMouseCursor::Normal };
}

//---------------//
//               //
//   RENDERING   //
//               //
//---------------//

void BoardEditor::draw(Scene& scene, ResourceId const& resource, int x, int y, Pen const& pen) const
{
    scene.add(resource, (x + 2) * TILE_SIZE, (y + 2) * TILE_SIZE, pen);
}

void BoardEditor::render(Scene& scene)
{
    Board const& board = game().board(board_id_);

    render_border(scene);
    for (intpos_t x = 0; x < board.w(); ++x)
        for (intpos_t y = 0; y < board.h(); ++y)
            render_tile(scene, x, y);
}

void BoardEditor::render_border(Scene& scene) const
{
    Board const& board = game().board(board_id_);

    draw(scene, board_top_left, -2, -2);
    draw(scene, board_top_right, board.w(), -2);
    draw(scene, board_bottom_left, -2, board.h());
    draw(scene, board_bottom_right, board.w(), board.h());

    for (ssize_t x = 0; x < board.w(); ++x) {
        draw(scene, board_top, x, -2);
        draw(scene, board_bottom, x, board.h());
    }

    for (ssize_t y = 0; y < board.h(); ++y) {
        draw(scene, board_left, -2, y);
        draw(scene, board_right, board.w(), y);
    }
}

void BoardEditor::render_tile(Scene& scene, intpos_t x, intpos_t y) const
{
    Board const& board = game().board(board_id_);

    draw(scene, tile, x, y);

    for (Direction const& dir: DIRECTIONS) {
        // draw wire
        auto it = board.wires().find({ board.id(), x, y, dir });
        if (it != board.wires().end())
            render_wire(scene, it->first, it->second, false);

        // draw temporary wire
        auto temp = board.temporary_wire();
        it = temp.find({ board.id(), x, y, dir });
        if (it != temp.end())
            render_wire(scene, it->first, it->second, true);

        auto itc = board.components().find({ board.id(), x, y });
        if (itc != board.components().end())
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
    static const std::unordered_map<std::tuple<Wire, Direction, bool>, resource_idx_t> wire_sprites {
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::N, true }, wire_top_on_north_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::S, true }, wire_top_on_south_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::W, true }, wire_top_on_west_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::E, true }, wire_top_on_east_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::N, false }, wire_top_on_north_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::S, false }, wire_top_on_south_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::W, false }, wire_top_on_west_1 },
        { { { Wire::Width::W1, Wire::Layer::Top }, Direction::E, false }, wire_top_on_east_1 },
    };

    auto it = wire_sprites.find({ wire, pos.dir, game().sandbox().wire_value(pos) });
    if (it == wire_sprites.end())
        throw std::runtime_error("Wire configuration not found");
    draw(scene, it->second, pos.x, pos.y, { .semitransparent = semitransparent });
}

void BoardEditor::render_component(Scene& scene, Position const& pos, Component const& component) const
{
    component.def->render(component, scene, (pos.x + 2) * TILE_SIZE, (pos.y + 2) * TILE_SIZE);
}
