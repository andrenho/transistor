#ifndef BOARDEDITOR_HH
#define BOARDEDITOR_HH

#include "engine/board/board.hh"
#include "../deviceeditor.hh"
#include "engine/sandbox/sandbox.hh"
#include "ui/scene.hh"

class BoardEditor : public DeviceEditor {
public:
    explicit BoardEditor(size_t board_id);

    static void load_icons();

    void render(Scene& scene) override;

    void on_mouse_press(int x, int y, uint8_t button, bool dbl_click) override;
    void on_mouse_release(int x, int y, uint8_t button) override;
    void on_mouse_move(int x, int y, int rx, int ry) override;
    void on_key_press(uint32_t key, int x, int y) override;
    void on_key_release(uint32_t key, int x, int y) override;

private:
    size_t       board_id_;
    bool         drawing_wire_ = false;
    bool         erasing_ = false;

    void render_border(Scene& scene) const;
    void render_tile(Scene& scene, intpos_t x, intpos_t y) const;
    void render_wire(Scene& scene, Position const& pos, Wire const& wire, bool semitransparent) const;
    void render_component(Scene& scene, Position const& pos, Component const& component) const;

    void draw(Scene& scene, std::string const& resource, int x, int y, Pen const& pen={}) const;

    void start_erasing(Position const& pos);
    void stop_erasing();
};

#endif //BOARDEDITOR_HH
