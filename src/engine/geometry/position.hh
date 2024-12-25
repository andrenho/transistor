#ifndef TILEPOS_HH
#define TILEPOS_HH

#include <ostream>

#include "direction.hh"
#include "engine/types.hh"

struct Position {
    intpos_t  x, y;
    Direction dir = Direction::Center;

    bool operator==(Position const& p) const { return x == p.x && y == p.y && dir == p.dir; }
    bool operator<(Position const& p) const;

    constexpr std::array<Position, 4> neighbours() const;
};

template<>
struct std::hash<Position> {
    std::size_t operator()(Position const& p) const noexcept
    {
        return (std::hash<Direction>{}(p.dir) << 32) ^ (std::hash<intpos_t>{}(p.x) << 16) ^ std::hash<intpos_t>{}(p.y);
    }
};

inline std::ostream& operator<<(std::ostream& os, Position const& p)
{
    return os << "<" << p.x << "," << p.y << "," << p.dir << ">";
}

inline bool Position::operator<(Position const& p) const
{
    static std::hash<Position> pfn;
    return pfn(*this) < pfn(p);
}

constexpr std::array<Position, 4> Position::neighbours() const
{
    switch (dir) {
        case Direction::W:
            return {{
                { x - 1, y, Direction::E },
                { x, y, Direction::E },
                { x, y, Direction::N },
                { x, y, Direction::S },
            }};
        case Direction::E:
            return {{
                { x + 1, y, Direction::W },
                { x, y, Direction::W },
                { x, y, Direction::N },
                { x, y, Direction::S },
            }};
        case Direction::N:
            return {{
                { x, y - 1, Direction::S },
                { x, y, Direction::S },
                { x, y, Direction::W },
                { x, y, Direction::E }
            }};
        case Direction::S:
            return {{
                { x, y + 1, Direction::N },
                { x, y, Direction::S },
                { x, y, Direction::W },
                { x, y, Direction::E },
            }};
    }

    throw std::runtime_error("Invalid direction");
}

#endif //TILEPOS_HH
