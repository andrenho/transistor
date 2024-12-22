#include "direction.hh"

#include <stdexcept>

Direction rotate(Direction dir)
{
    switch (dir) {
        case Direction::N: return Direction::W;
        case Direction::W: return Direction::S;
        case Direction::S: return Direction::E;
        case Direction::E: return Direction::N;
        case Direction::Center: return Direction::Center;
        default: throw std::runtime_error("Shouldn't happen");
    }
}

