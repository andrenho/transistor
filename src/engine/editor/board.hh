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
    Board(size_t w, size_t h, ComponentDatabase const& component_db) : w_(w), h_(h), component_db_(component_db) {}

    [[nodiscard]] std::map<Position, Component> const& components() const { return components_; }
    [[nodiscard]] std::map<Position, Wire> const& wires() const { return wires_; }

    void add_component(std::string const& component_name, intpos_t x, intpos_t y);
    void draw_wire(Wire::Width width, Wire::Layer layer, intpos_t x0, intpos_t y0, intpos_t x1, intpos_t y1, Orientation orientation);

private:
    size_t                   w_, h_;
    ComponentDatabase const& component_db_;

    std::map<Position, Component> components_;
    std::map<Position, Wire>      wires_;
    WireManagement                wire_management_;
};

#endif //BOARD_HH
