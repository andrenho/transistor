#ifndef RESOURCES_HH
#define RESOURCES_HH

#include <pastel2d.hh>

#define N_DIRS 5

extern ps::res::idx_t
    rs_bg,
    rs_tile,
    rs_board_top_left, rs_board_top, rs_board_top_right, rs_board_left, rs_board_right, rs_board_bottom_left, rs_board_bottom, rs_board_bottom_right,
    rs_wire_top_1[N_DIRS][2], rs_ic_dot[N_DIRS], rs_ic_pin[N_DIRS];

void load_resources();

#endif //RESOURCES_HH
