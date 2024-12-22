#ifndef BOARD_HH
#define BOARD_HH

#include <unordered_map>

#include "component.hh"
#include "wire.hh"
#include "engine/geometry/position.hh"

class Board {
public:
    [[nodiscard]] std::unordered_map<Position, Component> const& components() const { return components_; }
    [[nodiscard]] std::unordered_map<Position, Wire> const&      wires() const { return wires_; }

private:
    std::unordered_map<Position, Component> components_;
    std::unordered_map<Position, Wire>      wires_;
};

#endif //BOARD_HH
