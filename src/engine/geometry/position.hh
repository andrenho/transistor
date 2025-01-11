#ifndef TILEPOS_HH
#define TILEPOS_HH

#include <ostream>
#include <vector>

#include "direction.hh"
#include "engine/types.hh"

struct Position {
    Position(size_t _board_id, intpos_t _x, intpos_t _y, Direction _dir = Direction::Center)
        : board_id(_board_id), x(_x), y(_y), dir(_dir) {}

    Position(struct Board* board, intpos_t x, intpos_t y, Direction dir = Direction::Center);

    size_t    board_id;
    intpos_t  x, y;
    Direction dir;

    bool operator==(Position const& p) const { return board_id == p.board_id && x == p.x && y == p.y && dir == p.dir; }
    bool operator<(Position const& p) const;

    std::vector<Position> neighbours(bool has_single_tile_component_pin) const;
};

namespace std {
std::string to_string(Position const& pos);
}

template<>
struct std::hash<Position> {
    std::size_t operator()(Position const& p) const noexcept
    {
        return (std::hash<Direction>{}(p.dir) << 48) ^ (std::hash<intpos_t>{}(p.x) << 32) ^ (std::hash<intpos_t>{}(p.y) << 16) ^ std::hash<size_t>{}(p.board_id);
    }
};

inline std::ostream& operator<<(std::ostream& os, Position const& p)
{
    os << "<" << p.board_id << "|" << p.x << "," << p.y << "," << p.dir << ">";
    return os;
}

inline bool Position::operator<(Position const& p) const
{
    static std::hash<Position> pfn;
    return pfn(*this) < pfn(p);
}


#endif //TILEPOS_HH
