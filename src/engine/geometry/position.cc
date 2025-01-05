#include "position.hh"

#include "engine/editor/board.hh"

Position::Position(Board* board, intpos_t _x, intpos_t _y, Direction _dir)
    : board_id(board->id()), x(_x), y(_y), dir(_dir)
{
}

const std::array<Position, 4> Position::neighbours() const
{
    switch (dir) {
        case Direction::W:
            return {{
                { board_id, (intpos_t) (x - 1), y, Direction::E },
                { board_id, x, y, Direction::E },
                { board_id, x, y, Direction::N },
                { board_id, x, y, Direction::S },
            }};
        case Direction::E:
            return {{
                { board_id, (intpos_t) (x + 1), y, Direction::W },
                { board_id, x, y, Direction::W },
                { board_id, x, y, Direction::N },
                { board_id, x, y, Direction::S },
            }};
        case Direction::N:
            return {{
                { board_id, x, (intpos_t) (y - 1), Direction::S },
                { board_id, x, y, Direction::S },
                { board_id, x, y, Direction::W },
                { board_id, x, y, Direction::E }
            }};
        case Direction::S:
            return {{
                { board_id, x, (intpos_t) (y + 1), Direction::N },
                { board_id, x, y, Direction::N },
                { board_id, x, y, Direction::W },
                { board_id, x, y, Direction::E },
            }};
        case Direction::Center:
        default:
            throw std::runtime_error("Invalid direction");
    }
}
