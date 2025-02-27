#ifndef RESOURCES_H
#define RESOURCES_H

#include "pastel2d.h"

extern ps_res_idx_t
    rs_tile,
    rs_board_top_left, rs_board_top, rs_board_top_right, rs_board_left, rs_board_right, rs_board_bottom_left, rs_board_bottom, rs_board_bottom_right,
    rs_wire_top_on_1[5], rs_wire_top_off_1[5], rs_ic_dot[5], rs_ic_pin[5];

void load_resources();

#endif //RESOURCES_H
