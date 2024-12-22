#ifndef DIRECTION_HH
#define DIRECTION_HH

#include <cstdint>
#include <ostream>

enum class Direction : uint8_t {
    N = 'N', S = 'S', W = 'W', E='E', Center='X'
};

Direction rotate(Direction dir);

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
