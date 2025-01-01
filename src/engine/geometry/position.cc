#include "position.hh"

#include "engine/editor/board.hh"

Position::Position(Board* board, intpos_t _x, intpos_t _y, Direction _dir)
    : board_id(board->id()), x(_x), y(_y), dir(_dir)
{
}
