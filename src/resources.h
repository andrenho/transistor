#ifndef RESOURCES_H
#define RESOURCES_H

#include "pastel2d.h"
#include "transistor-sandbox.h"

extern ps_res_idx_t
    rs_tile,
    rs_board_top_left, rs_board_top, rs_board_top_right, rs_board_left, rs_board_right, rs_board_bottom_left, rs_board_bottom, rs_board_bottom_right,
    rs_wire_top_1[TS_DIRS_N][2], rs_ic_dot[TS_DIRS_N], rs_ic_pin[TS_DIRS_N];

void load_resources();

#endif //RESOURCES_H
