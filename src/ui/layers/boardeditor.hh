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

    void on_mouse_press(UI_Interface& uif, int x, int y, uint8_t button, bool dbl_click) override;

private:
    Board& board_;
    std::vector<Resource> icons_;

    void render_border(UI_Interface const& uif) const;
    void render_tile(UI_Interface const& uif, int x, int y) const;

    void draw(UI_Interface const& uif, CSprite sprite, int x, int y, DrawProperties dp={}) const;
};

#endif //BOARDEDITOR_HH
