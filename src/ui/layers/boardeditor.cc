#include "boardeditor.hh"

#include "ui/circuit_atlas.hh"

static Resource tile;  // TODO

BoardEditor::BoardEditor(ResourceManager& resource_manager, Resource const& circuit, Board& board)
    : UILayer(0, 0, 100, 100 /* TODO */), board_(board)
{
    tile = resource_manager.from_tileset(circuit, TILE_SZ, 3, 1);
}

void BoardEditor::render(DrawF draw)
{
    draw(tile, 0, 0, {});
}
