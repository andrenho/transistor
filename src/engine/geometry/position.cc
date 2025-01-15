#include "position.hh"

#include <sstream>

#include "engine/board/board.hh"

Position::Position(Board* board, intpos_t _x, intpos_t _y, Direction _dir)
    : board_id(board->id()), x(_x), y(_y), dir(_dir)
{
}

Position::Position(std::string const& from)
{
    std::stringstream ss(from);
    std::string temp;

    getline(ss, temp, ','); board_id = std::stoi(temp);
    getline(ss, temp, ','); x = (intpos_t) std::stoi(temp);
    getline(ss, temp, ','); y = (intpos_t) std::stoi(temp);
    getline(ss, temp);            dir = (Direction) temp[0];
}

std::vector<Position> Position::neighbours(bool has_single_tile_component_pin) const
{
    if (has_single_tile_component_pin) {
        switch (dir) {
            case Direction::W:
                return { { board_id, (intpos_t) (x - 1), y, Direction::E } };
            case Direction::E:
                return { { board_id, (intpos_t) (x + 1), y, Direction::W } };
            case Direction::N:
                return { { board_id, x, (intpos_t) (y - 1), Direction::S } };
            case Direction::S:
                return { { board_id, x, (intpos_t) (y + 1), Direction::N } };
            case Direction::Center:
                default:
                    throw std::runtime_error("Invalid direction");
        }
    } else {
        switch (dir) {
            case Direction::W:
                return {
                    { board_id, (intpos_t) (x - 1), y, Direction::E },
                    { board_id, x, y, Direction::E },
                    { board_id, x, y, Direction::N },
                    { board_id, x, y, Direction::S },
                };
            case Direction::E:
                return {
                    { board_id, (intpos_t) (x + 1), y, Direction::W },
                    { board_id, x, y, Direction::W },
                    { board_id, x, y, Direction::N },
                    { board_id, x, y, Direction::S },
                };
            case Direction::N:
                return {
                    { board_id, x, (intpos_t) (y - 1), Direction::S },
                    { board_id, x, y, Direction::S },
                    { board_id, x, y, Direction::W },
                    { board_id, x, y, Direction::E }
                };
            case Direction::S:
                return {
                    { board_id, x, (intpos_t) (y + 1), Direction::N },
                    { board_id, x, y, Direction::N },
                    { board_id, x, y, Direction::W },
                    { board_id, x, y, Direction::E },
                };
            case Direction::Center:
                default:
                    throw std::runtime_error("Invalid direction");
        }
    }
}

namespace std {

std::string to_string(Position const& pos)
{
    return std::to_string(pos.board_id) + "," + std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::string(1, (char) pos.dir);
}

}
