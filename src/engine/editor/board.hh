#ifndef BOARD_HH
#define BOARD_HH

#include <unordered_map>

#include "component.hh"
#include "wire.hh"
#include "engine/geometry/position.hh"

class Board {
public:
    Board(size_t w, size_t h) : w_(w), h_(h) {}

    [[nodiscard]] std::unordered_map<Position, Component> const& components() const { return components_; }
    [[nodiscard]] std::unordered_map<Position, Wire> const&      wires() const { return wires_; }

    void add_component(std::string const& component_name, intpos_t x, intpos_t y);

private:
    size_t w_, h_;
    std::unordered_map<Position, Component> components_;
    std::unordered_map<Position, Wire>      wires_;
};

#endif //BOARD_HH
