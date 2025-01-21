#ifndef BOARD_HH
#define BOARD_HH

#include <map>
#include <optional>

#include "component.hh"
#include "wire.hh"
#include "wiremanagement.hh"
#include "engine/componentdb/componentdb.hh"
#include "engine/geometry/position.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Board {
public:
    Board(intpos_t w, intpos_t h, ComponentDatabase const& component_db);
    Board(json const& content, ComponentDatabase const& component_db);

    [[nodiscard]] std::map<Position, Component> const& components() const { return components_; }
    [[nodiscard]] std::map<Position, Component>& components() { return components_; }
    [[nodiscard]] std::map<Position, Wire> const& wires() const { return wires_; }

    std::optional<Component*> add_component(std::string const& component_name, intpos_t x, intpos_t y, Direction dir);
    void draw_wire(Wire::Width width, Wire::Layer layer, intpos_t x0, intpos_t y0, intpos_t x1, intpos_t y1, Orientation orientation);
    void merge_wires(std::map<Position, Wire> const& wires);

    void       clear_tile(intpos_t x, intpos_t y);
    void       clear();

    void       start_placing_wire(Wire::Width width, Wire::Layer layer, intpos_t x, intpos_t y) { wire_management_.start_drawing({ this, x, y }, width, layer); }
    void       continue_placing_wire(intpos_t x, intpos_t y) { wire_management_.set_current_end({ this, x, y }); }
    void       finish_placing_wire(intpos_t x, intpos_t y) { merge_wires(wire_management_.stop_drawing({ this, x, y })); }
    void       cancel_placing_wire() { wire_management_.stop_drawing(); }

    void       rotate_component(intpos_t x, intpos_t y);

    [[nodiscard]] json serialize() const;

    [[nodiscard]] std::map<Position, Wire> temporary_wire() const { return wire_management_.current_drawing(); }

    [[nodiscard]] intpos_t w() const { return w_; }
    [[nodiscard]] intpos_t h() const { return h_; }

    [[nodiscard]] size_t id() const { return id_; }

    friend bool operator==(Board const& lhs, Board const& rhs) { return std::tie(lhs.id_, lhs.w_, lhs.h_, lhs.components_, lhs.wires_) == std::tie(rhs.id_, rhs.w_, rhs.h_, rhs.components_, rhs.wires_); }
    friend bool operator!=(Board const& lhs, Board const& rhs) { return !(lhs == rhs); }

private:
    const size_t             id_;
    intpos_t                 w_, h_;
    ComponentDatabase const& component_db_;

    std::map<Position, Component> components_;
    std::map<Position, Wire>      wires_;
    WireManagement                wire_management_;

    void remove_wires_for_ic(Position const& r1, Position const& r2);

    static size_t board_counter_;
};

#endif //BOARD_HH
