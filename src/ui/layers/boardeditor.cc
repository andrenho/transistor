#include "boardeditor.hh"

#include "battery/embed.hpp"
#include "ui/resources/circuit_atlas.hh"
#include "ui/uiinterface.hh"

static Resource tile;  // TODO

BoardEditor::BoardEditor(ResourceManager& resource_manager, Board& board)
    : UILayer(0, 0, (board.w() + 4) * TILE_SIZE, (board.w() + 4) * TILE_SIZE), board_(board)
{
    Resource circuit = resource_manager.from_image(b::embed<"resources/images/circuit.png">().vec());
    icons_ = resource_manager.from_atlas(circuit, circuit_coordinates, TILE_SIZE);
    zoom = 2.f;
}

void BoardEditor::draw(UI_Interface const& uif, CSprite sprite, int x, int y, DrawProperties dp) const
{
    uif.draw_image(this, icons_.at(static_cast<size_t>(sprite)), x, y, dp);
}

void BoardEditor::render(UI_Interface const& uif)
{
    draw(uif, CSprite::BoardTopLeft, 0, 0);
    draw(uif, CSprite::BoardTopRight, (board_.w() + 2) * TILE_SIZE, 0);
    draw(uif, CSprite::BoardBottomLeft, 0, (board_.h() + 2) * TILE_SIZE);
    draw(uif, CSprite::BoardBottomRight, (board_.w() + 2) * TILE_SIZE, (board_.h() + 2) * TILE_SIZE);

    for (ssize_t x = 0; x < board_.w(); ++x) {
        draw(uif, CSprite::BoardTop, (x + 2) * TILE_SIZE, 0);
        draw(uif, CSprite::BoardBottom, (x + 2) * TILE_SIZE, (board_.h() + 2) * TILE_SIZE);
    }

    for (ssize_t y = 0; y < board_.h(); ++y) {
        draw(uif, CSprite::BoardLeft, 0 * TILE_SIZE, (y + 2) * TILE_SIZE);
        draw(uif, CSprite::BoardRight, (board_.w() + 2) * TILE_SIZE, (y + 2) * TILE_SIZE);
    }
}
