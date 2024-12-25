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
    void on_mouse_release(UI_Interface& uif, int x, int y, uint8_t button) override;
    void on_mouse_move(UI_Interface& uif, int x, int y, int rx, int ry) override;
    void on_key_press(UI_Interface& uif, uint32_t key, int x, int y) override;
    void on_key_release(UI_Interface& uif, uint32_t key, int x, int y) override;

private:
    Board&                board_;
    std::vector<Resource> icons_;
    bool                  drawing_wire_ = false;

    void render_border(UI_Interface const& uif) const;
    void render_tile(UI_Interface const& uif, intpos_t x, intpos_t y) const;
    void render_wire(UI_Interface const& uif, Position const& pos, Wire const& wire, bool semitransparent) const;

    void draw(UI_Interface const& uif, CSprite sprite, int x, int y, DrawProperties dp={}) const;
};

#endif //BOARDEDITOR_HH
