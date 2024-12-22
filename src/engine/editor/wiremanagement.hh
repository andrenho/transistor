#ifndef WIREMANAGEMENT_HH
#define WIREMANAGEMENT_HH

#include <optional>
#include <map>

#include "wire.hh"
#include "engine/geometry/position.hh"

class WireManagement {
public:
    void                     start_drawing(Position const& pos, Wire::Width width, Wire::Layer side);
    void                     set_current_end(Position const& pos);
    std::map<Position, Wire> stop_drawing(Position const& pos);

    void    set_orientation(Orientation orientation) { if (drawing_wire_) drawing_wire_->orientation = orientation; }

    [[nodiscard]] std::map<Position, Wire> current_drawing() const;

private:
    struct TempWire {
        Position                   start_pos;
        Position                   end_pos;
        Wire::Width                width;
        Wire::Layer                side;
        std::optional<Orientation> orientation {};
    };
    std::optional<TempWire> drawing_wire_;
};

#endif //WIREMANAGEMENT_HH
