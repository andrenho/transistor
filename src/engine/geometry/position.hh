#ifndef TILEPOS_HH
#define TILEPOS_HH
#include <cstdint>

#include "direction.hh"
#include "engine/types.hh"

struct Position {
    intpos_t  x, y;
    Direction dir;

    bool operator==(Position const& p) const { return x == p.x && y == p.y && dir == p.dir; }
    bool operator<(Position const& p) const;
};

template<>
struct std::hash<Position> {
    std::size_t operator()(Position const& p) const noexcept
    {
        return (std::hash<Direction>{}(p.dir) << 1) ^ (std::hash<intpos_t>{}(p.x) << 1) ^ std::hash<intpos_t>{}(p.y);
    }
};

inline bool Position::operator<(Position const& p) const
{
    static std::hash<Position> pfn;
    return pfn(*this) < pfn(p);
}


#endif //TILEPOS_HH
