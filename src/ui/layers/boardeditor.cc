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
    draw(uif, CSprite::Tile, 0, 0);
}
