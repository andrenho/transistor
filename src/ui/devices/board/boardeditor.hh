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

    struct IC_Resource { resource_idx_t nw = -1, n = -1, ne = -1, e = -1, w = -1, sw = -1, s = -1, se = -1, center = -1; };

private:
    size_t       board_id_;
    bool         drawing_wire_ = false;
    bool         erasing_ = false;

    void render_border(Scene& scene) const;
    void render_tile(Scene& scene, intpos_t x, intpos_t y) const;
    void render_wire(Scene& scene, Position const& pos, Wire const& wire, bool semitransparent) const;
    void render_component(Scene& scene, Position const& pos, Component const& component) const;
    void render_ic_shell(Scene const& scene, Position pos, ComponentDefinition const* def, Direction rotation, bool semitransparent=false) const;
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
        ic_pin_n, ic_pin_w, ic_pin_e, ic_pin_s;
    static std::unordered_map<ComponentDefinition::Category, IC_Resource> ic_res;
};

#endif //BOARDEDITOR_HH
