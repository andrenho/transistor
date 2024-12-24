#include "boardeditor.hh"

#include "battery/embed.hpp"
#include "ui/resources/circuit_atlas.hh"

static Resource tile;  // TODO

BoardEditor::BoardEditor(ResourceManager& resource_manager, Board& board)
    : UILayer(0, 0, (board_.w() + 4) * TILE_SIZE, (board_.w() + 4) * TILE_SIZE), board_(board)
{
    Resource circuit = resource_manager.from_image(b::embed<"resources/images/circuit.png">().vec());
    icons_ = resource_manager.from_atlas(circuit, circuit_coordinates, TILE_SIZE);
    zoom = 2.f;
}

void BoardEditor::render(DrawF draw)
{
    auto draw_icon = [this, &draw](CSprite sprite, int x, int y, DrawProperties dp={}) {
        draw(icons_.at((size_t) sprite), x, y, dp);
    };

    draw_icon(CSprite::Tile, 0, 0);
}
