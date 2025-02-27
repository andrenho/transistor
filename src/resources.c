#include "resources.h"

#include "transistor-sandbox.h"

// embedded files
#include "circuit.png.h"
#include "circuit.tileset.lua.h"

extern ps_res_idx_t
    rs_tile,
    rs_board_top_left, rs_board_top, rs_board_top_right, rs_board_left, rs_board_right, rs_board_bottom_left, rs_board_bottom, rs_board_bottom_right,
    rs_wire_top_on_1[5], rs_wire_top_off_1[5], rs_ic_dot[5], rs_ic_pin[5];

void load_resources()
{
    ps_res_idx_t circuit = PS_ASSERT_RES(ps_res_add_png(resources_images_circuit_png, resources_images_circuit_png_sz));
    PS_ASSERT(ps_res_add_tiles_from_lua(circuit, resources_images_circuit_tileset_lua, resources_images_circuit_tileset_lua_sz));

    rs_tile = PS_IDX("tile");

    rs_board_top_left = PS_IDX("board_top_left");
    rs_board_top = PS_IDX("board_top");
    rs_board_top_right = PS_IDX("board_top_right");
    rs_board_left = PS_IDX("board_left");
    rs_board_right = PS_IDX("board_right");
    rs_board_bottom_left = PS_IDX("board_bottom_left");
    rs_board_bottom = PS_IDX("board_bottom");
    rs_board_bottom_right = PS_IDX("board_bottom_right");

}
