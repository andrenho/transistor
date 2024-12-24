#ifndef BOARDEDITOR_HH
#define BOARDEDITOR_HH

#include "engine/editor/board.hh"
#include "ui/ui_layer.hh"
#include "ui/resources/circuit_atlas.hh"
#include "ui/resources/resourcemanager.hh"

class BoardEditor : public UILayer {
public:
    BoardEditor(ResourceManager& resource_manager, Board& board);

    void render(UI_Interface const& uif) override;

private:
    Board& board_;
    std::vector<Resource> icons_;

    void draw(UI_Interface const& uif, CSprite sprite, int x, int y, DrawProperties dp={}) const;
};

#endif //BOARDEDITOR_HH
