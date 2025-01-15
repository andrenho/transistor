#ifndef BOARDEDITOR_HH
#define BOARDEDITOR_HH

#include "engine/editor/board.hh"
#include "../deviceeditor.hh"
#include "circuit_atlas.hh"
#include "engine/sandbox/sandbox.hh"
#include "ui/scene.hh"

class BoardEditor : public DeviceEditor {
public:
    BoardEditor(ResourceManager& resource_manager, Game const& game, size_t board_id);

    void render(Game const& game, Scene& scene) override;

    void on_mouse_press(Game const& game, int x, int y, uint8_t button, bool dbl_click, Events& events) override;
    void on_mouse_release(Game const& game, int x, int y, uint8_t button, Events& events) override;
    void on_mouse_move(Game const& game, int x, int y, int rx, int ry, Events& events) override;
    void on_key_press(Game const& game, uint32_t key, int x, int y, Events& events) override;
    void on_key_release(Game const& game, uint32_t key, int x, int y, Events& events) override;

private:
    Board const& board_;
    bool         drawing_wire_ = false;
    bool         erasing_ = false;

    void render_border(Scene& scene) const;
    void render_tile(Scene& scene, intpos_t x, intpos_t y) const;
    void render_wire(Scene& scene, Position const& pos, Wire const& wire, bool semitransparent) const;
    void render_component(Scene& scene, Position const& pos, Component const& component) const;

    void draw(Scene& scene, CSprite sprite, int x, int y, Pen const& pen={}) const;

    void start_erasing(Game const& game, Position const& pos, Events& events);
    void stop_erasing(Events& events);
};

#endif //BOARDEDITOR_HH
