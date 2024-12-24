#ifndef BOARDEDITOR_HH
#define BOARDEDITOR_HH

#include "engine/editor/board.hh"
#include "ui/ui_layer.hh"
#include "ui/resources/resourcemanager.hh"

class BoardEditor : public UILayer {
public:
    BoardEditor(ResourceManager& resource_manager, Resource const& circuit, Board& board);

    void render(DrawF draw) override;

private:
    Board& board_;
};

#endif //BOARDEDITOR_HH
