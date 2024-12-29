#ifndef BOARD_HH
#define BOARD_HH

#include <map>

#include "component.hh"
#include "wire.hh"
#include "wiremanagement.hh"
#include "engine/componentdb/componentdb.hh"
#include "engine/geometry/position.hh"

class Board {
public:
    Board(intpos_t w, intpos_t h, class Sandbox& sandbox, ComponentDatabase const& component_db)
        : w_(w), h_(h), sandbox_(sandbox), component_db_(component_db) {}

    [[nodiscard]] std::map<Position, Component> const& components() const { return components_; }
    [[nodiscard]] std::map<Position, Component>& components() { return components_; }
    [[nodiscard]] std::map<Position, Wire> const& wires() const { return wires_; }

    Component* add_component(std::string const& component_name, intpos_t x, intpos_t y);
    void       draw_wire(Wire::Width width, Wire::Layer layer, intpos_t x0, intpos_t y0, intpos_t x1, intpos_t y1, Orientation orientation);
    void       merge_wires(std::map<Position, Wire> const& wires);

    void       clear_tile(intpos_t x, intpos_t y);

    void       start_placing_wire(Wire::Width width, Wire::Layer layer, intpos_t x, intpos_t y) { wire_management_.start_drawing({ x, y }, width, layer); }
    void       continue_placing_wire(intpos_t x, intpos_t y) { wire_management_.set_current_end({ x, y }); }
    void       finish_placing_wire(intpos_t x, intpos_t y) { merge_wires(wire_management_.stop_drawing({ x, y })); }
    void       cancel_placing_wire() { wire_management_.stop_drawing(); }
    [[nodiscard]] std::map<Position, Wire> temporary_wire() const { return wire_management_.current_drawing(); }

    [[nodiscard]] intpos_t w() const { return w_; }
    [[nodiscard]] intpos_t h() const { return h_; }

private:
    intpos_t                 w_, h_;
    Sandbox&                 sandbox_;
    ComponentDatabase const& component_db_;

    std::map<Position, Component> components_;
    std::map<Position, Wire>      wires_;
    WireManagement                wire_management_;
};

#endif //BOARD_HH
