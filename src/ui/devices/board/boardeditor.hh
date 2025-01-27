#ifndef BOARDEDITOR_HH
#define BOARDEDITOR_HH

#include "engine/board/board.hh"
#include "../deviceeditor.hh"
#include "engine/sandbox/sandbox.hh"
#include "res/resourcemanager.hh"
#include "ui/scene.hh"

class BoardEditor : public DeviceEditor {
public:
    BoardEditor(size_t board_id, int x, int y);

    static void load_icons();

    void render(Scene& scene, int mx, int my) override;

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
    void render_cursor(Scene& scene, int mx, int my) const;

    void draw(Scene& scene, ResourceId const& resource, int x, int y, Pen const& pen={}) const;

    void start_erasing(Position const& pos);
    void stop_erasing();

    std::optional<ComponentDefinition const*> selected_component_definition() const;

    static resource_idx_t
        tile,
        board_top_left, board_top, board_top_right, board_left, board_right, board_bottom_left, board_bottom, board_bottom_right,
        wire_top_on_north_1, wire_top_on_east_1, wire_top_on_west_1, wire_top_on_south_1,
        wire_top_off_north_1, wire_top_off_east_1, wire_top_off_west_1, wire_top_off_south_1,
        ic_dot_n, ic_dot_w, ic_dot_e, ic_dot_s,
        ic_pin_n, ic_pin_w, ic_pin_e, ic_pin_s,
        ic_logic_nw, ic_logic_n, ic_logic_ne, ic_logic_e, ic_logic_w, ic_logic_sw, ic_logic_s, ic_logic_se, ic_logic_center,
        ic_digital_nw, ic_digital_n, ic_digital_ne, ic_digital_e, ic_digital_w, ic_digital_sw, ic_digital_s, ic_digital_se, ic_digital_center,
        ic_memory_nw, ic_memory_n, ic_memory_ne, ic_memory_e, ic_memory_w, ic_memory_sw, ic_memory_s, ic_memory_se, ic_memory_center,
        ic_cpu_nw, ic_cpu_n, ic_cpu_ne, ic_cpu_e, ic_cpu_w, ic_cpu_sw, ic_cpu_s, ic_cpu_se, ic_cpu_center;
};

#endif //BOARDEDITOR_HH
