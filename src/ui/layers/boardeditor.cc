#include "boardeditor.hh"

#include <iostream>

#include "battery/embed.hpp"
#include "ui/resources/circuit_atlas.hh"
#include "ui/uiinterface.hh"

BoardEditor::BoardEditor(ResourceManager& resource_manager, Board& board)
    : UILayer(0, 0, (board.w() + 4) * TILE_SIZE, (board.w() + 4) * TILE_SIZE), board_(board)
{
    Resource circuit = resource_manager.from_image(b::embed<"resources/images/circuit.png">().vec());
    icons_ = resource_manager.from_atlas(circuit, circuit_coordinates, TILE_SIZE);
    zoom = 2.f;
}

void BoardEditor::draw(UI_Interface const& uif, CSprite sprite, int x, int y, DrawProperties dp) const
{
    uif.draw_image(this, icons_.at(static_cast<size_t>(sprite)), x * TILE_SIZE, y * TILE_SIZE, dp);
}

void BoardEditor::render(UI_Interface const& uif)
{
    render_border(uif);
    for (int x = 0; x < board_.w(); ++x)
        for (int y = 0; y < board_.h(); ++y)
            render_tile(uif, x, y);
}

void BoardEditor::render_border(UI_Interface const& uif) const
{
    draw(uif, CSprite::BoardTopLeft, 0, 0);
    draw(uif, CSprite::BoardTopRight, board_.w() + 2, 0);
    draw(uif, CSprite::BoardBottomLeft, 0, board_.h() + 2);
    draw(uif, CSprite::BoardBottomRight, board_.w() + 2, board_.h() + 2);

    for (ssize_t x = 0; x < board_.w(); ++x) {
        draw(uif, CSprite::BoardTop, x + 2, 0);
        draw(uif, CSprite::BoardBottom, x + 2, board_.h() + 2);
    }

    for (ssize_t y = 0; y < board_.h(); ++y) {
        draw(uif, CSprite::BoardLeft, 0, y + 2);
        draw(uif, CSprite::BoardRight, board_.w() + 2, y + 2);
    }
}

void BoardEditor::render_tile(UI_Interface const& uif, int x, int y) const
{
    draw(uif, CSprite::Tile, x + 2, y + 2);
}

void BoardEditor::on_mouse_press(UI_Interface& uif, int x, int y, uint8_t button, bool dbl_click)
{
    int tx = x / TILE_SIZE - 2;
    int ty = y / TILE_SIZE - 2;
    std::cout << tx << ", " << ty << "\n";
}
