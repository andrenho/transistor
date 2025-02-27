#include "resources.h"

// embedded files
#include "bg.png.h"
#include "circuit.png.h"
#include "circuit.tileset.lua.h"

ps_res_idx_t
    rs_bg,
    rs_tile,
    rs_board_top_left, rs_board_top, rs_board_top_right, rs_board_left, rs_board_right, rs_board_bottom_left, rs_board_bottom, rs_board_bottom_right,
    rs_wire_top_1[TS_DIRS_N][2], rs_ic_dot[TS_DIRS_N], rs_ic_pin[TS_DIRS_N];

void load_resources()
{
    rs_bg = PS_ASSERT_RES(ps_res_add_png(resources_images_bg_png, resources_images_bg_png_sz));

    // load circuit.png tileset
    ps_res_idx_t circuit = PS_ASSERT_RES(ps_res_add_png(resources_images_circuit_png, resources_images_circuit_png_sz));
    PS_ASSERT(ps_res_add_tiles_from_lua(circuit, resources_images_circuit_tileset_lua, resources_images_circuit_tileset_lua_sz));

    // transform string indexes into numeric indexes for faster access

    rs_tile = PS_IDX("tile");

    rs_board_top_left = PS_IDX("board_top_left");
    rs_board_top = PS_IDX("board_top");
    rs_board_top_right = PS_IDX("board_top_right");
    rs_board_left = PS_IDX("board_left");
    rs_board_right = PS_IDX("board_right");
    rs_board_bottom_left = PS_IDX("board_bottom_left");
    rs_board_bottom = PS_IDX("board_bottom");
    rs_board_bottom_right = PS_IDX("board_bottom_right");

    rs_wire_top_1[TS_N][true] = PS_IDX("wire_top_on_north_1");
    rs_wire_top_1[TS_E][true] = PS_IDX("wire_top_on_east_1");
    rs_wire_top_1[TS_W][true] = PS_IDX("wire_top_on_west_1");
    rs_wire_top_1[TS_S][true] = PS_IDX("wire_top_on_south_1");
    rs_wire_top_1[TS_N][false] = PS_IDX("wire_top_off_north_1");
    rs_wire_top_1[TS_E][false] = PS_IDX("wire_top_off_east_1");
    rs_wire_top_1[TS_W][false] = PS_IDX("wire_top_off_west_1");
    rs_wire_top_1[TS_S][false] = PS_IDX("wire_top_off_south_1");

    rs_ic_dot[TS_N] = PS_IDX("ic_dot_n");
    rs_ic_dot[TS_W] = PS_IDX("ic_dot_w");
    rs_ic_dot[TS_E] = PS_IDX("ic_dot_e");
    rs_ic_dot[TS_S] = PS_IDX("ic_dot_s");

    rs_ic_pin[TS_N] = PS_IDX("ic_pin_n");
    rs_ic_pin[TS_W] = PS_IDX("ic_pin_w");
    rs_ic_pin[TS_E] = PS_IDX("ic_pin_e");
    rs_ic_pin[TS_S] = PS_IDX("ic_pin_s");
}
