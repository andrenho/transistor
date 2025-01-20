#ifndef TILEPOS_HH
#define TILEPOS_HH

#include <ostream>
#include <vector>

#include "direction.hh"
#include "engine/util/types.hh"

struct Position {
    Position(size_t _board_id, ssize_t _x, ssize_t _y, Direction _dir = Direction::Center)
        : board_id(_board_id), x(_x), y(_y), dir(_dir) {}

    Position(struct Board* board, ssize_t x, ssize_t y, Direction dir = Direction::Center);

    explicit Position(std::string const& from);

    size_t    board_id;
    intpos_t  x, y;
    Direction dir;

    bool        operator<(Position const& p) const;

    std::vector<Position> neighbours(bool has_single_tile_component_pin) const;
    Position add(intpos_t x_, intpos_t y_) const;

    friend bool operator==(Position const& lhs, Position const& rhs) { return std::tie(lhs.board_id, lhs.x, lhs.y, lhs.dir) == std::tie(rhs.board_id, rhs.x, rhs.y, rhs.dir); }
    friend bool operator!=(Position const& lhs, Position const& rhs) { return !(lhs == rhs); }
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
