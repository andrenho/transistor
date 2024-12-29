#ifndef BOARDEDITOR_HH
#define BOARDEDITOR_HH

#include "engine/editor/board.hh"
#include "../layer.hh"
#include "circuit_atlas.hh"
#include "ui/resources/resourcemanager.hh"

class BoardEditor : public Layer {
public:
    BoardEditor(ResourceManager& resource_manager, Board& board);

    void render(Scene& scene) override;

    void on_mouse_press(int x, int y, uint8_t button, bool dbl_click, Events& events) override;
    void on_mouse_release(int x, int y, uint8_t button, Events& events) override;
    void on_mouse_move(int x, int y, int rx, int ry, Events& events) override;
    void on_key_press(uint32_t key, int x, int y, Events& events) override;
    void on_key_release(uint32_t key, int x, int y, Events& events) override;

private:
    Board&                board_;
    std::vector<Resource> icons_;
    bool                  drawing_wire_ = false;
    bool                  erasing_wire_ = false;

    void render_border(Scene& scene) const;
    void render_tile(Scene& scene, intpos_t x, intpos_t y) const;
    void render_wire(Scene& scene, Position const& pos, Wire const& wire, bool semitransparent) const;
    void render_component(Scene& scene, Position const& pos, Component const& component) const;

    void draw(Scene& scene, CSprite sprite, int x, int y, Pen const& pen={}) const;
};

#endif //BOARDEDITOR_HH
