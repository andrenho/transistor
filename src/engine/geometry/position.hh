#ifndef TILEPOS_HH
#define TILEPOS_HH
#include <cstdint>

#include "direction.hh"

struct Position {
    int16_t   x, y;
    Direction dir;

    bool operator==(Position const& p) const { return x == p.x && y == p.y && dir == p.dir; }
};

template<>
struct std::hash<Position> {
    std::size_t operator()(Position const& p) const noexcept
    {
        return (std::hash<Direction>{}(p.dir) << 1) ^ (std::hash<ssize_t>{}(p.x) << 1) ^ std::hash<ssize_t>{}(p.y);
    }
};


#endif //TILEPOS_HH
