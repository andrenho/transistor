#ifndef DIRECTION_HH
#define DIRECTION_HH

#include <cstdint>
#include <ostream>

enum class Direction : uint8_t {
    N = 'N', S = 'S', W = 'W', E='E', Center='X'
};

constexpr Direction DIRECTIONS[] = { Direction::N, Direction::W, Direction::S, Direction::E };

constexpr Direction dir_rotate_component(Direction dir)
{
    switch (dir) {
        case Direction::N: return Direction::E;
        case Direction::E: return Direction::S;
        case Direction::S: return Direction::W;
        case Direction::W: return Direction::N;
        case Direction::Center: return Direction::Center;
        default: throw std::runtime_error("Shouldn't happen");
    }
}

constexpr double dir_angle_clockwise(Direction dir)
{
    switch (dir) {
        case Direction::N: return 0.0;
        case Direction::E: return 90.0;
        case Direction::S: return 180.0;
        case Direction::W: return 270.0;
        case Direction::Center: return 0.0;
        default: throw std::runtime_error("Shouldn't happen");
    }
}

inline std::ostream& operator<<(std::ostream& os, Direction dir) {
    switch (dir) {
        case Direction::N: return os << "N";
        case Direction::S: return os << "S";
        case Direction::W: return os << "W";
        case Direction::E: return os << "E";
        case Direction::Center: return os << "X";
        default: return os << "?";
    }
}

enum class Orientation: uint8_t { Horizontal, Vertical };

#endif //DIRECTION_HH
