#include "resources.hh"

#include <snapshot.hh>

#include "images/bg.jpg.h"
#include "images/circuit.png.h"
#include "images/circuit.tileset.lua.h"

ps::res::idx_t
    rs_bg,
    rs_tile,
    rs_board_top_left, rs_board_top, rs_board_top_right, rs_board_left, rs_board_right, rs_board_bottom_left, rs_board_bottom, rs_board_bottom_right,
    rs_wire_top_1[N_DIRS][2], rs_ic_dot[N_DIRS], rs_ic_pin[N_DIRS];

void load_resources()
{
    rs_bg = ps::res::add_image(ui_resources_images_bg_jpg, ui_resources_images_bg_jpg_sz);

    auto circuit = ps::res::add_image("circuit", ui_resources_images_circuit_png, ui_resources_images_circuit_png_sz);
    ps::res::add_tiles_from_lua(circuit, ui_resources_images_circuit_tileset_lua, ui_resources_images_circuit_tileset_lua_sz);

    rs_tile = ps::res::idx("tile");

    rs_board_top_left = ps::res::idx("board_top_left");
    rs_board_top = ps::res::idx("board_top");
    rs_board_top_right = ps::res::idx("board_top_right");
    rs_board_left = ps::res::idx("board_left");
    rs_board_right = ps::res::idx("board_right");
    rs_board_bottom_left = ps::res::idx("board_bottom_left");
    rs_board_bottom = ps::res::idx("board_bottom");
    rs_board_bottom_right = ps::res::idx("board_bottom_right");

    rs_wire_top_1[(size_t) Direction::N][true] = ps::res::idx("wire_top_on_north_1");
    rs_wire_top_1[(size_t) Direction::E][true] = ps::res::idx("wire_top_on_east_1");
    rs_wire_top_1[(size_t) Direction::W][true] = ps::res::idx("wire_top_on_west_1");
    rs_wire_top_1[(size_t) Direction::S][true] = ps::res::idx("wire_top_on_south_1");
    rs_wire_top_1[(size_t) Direction::N][false] = ps::res::idx("wire_top_off_north_1");
    rs_wire_top_1[(size_t) Direction::E][false] = ps::res::idx("wire_top_off_east_1");
    rs_wire_top_1[(size_t) Direction::W][false] = ps::res::idx("wire_top_off_west_1");
    rs_wire_top_1[(size_t) Direction::S][false] = ps::res::idx("wire_top_off_south_1");

    rs_ic_dot[(size_t) Direction::N] = ps::res::idx("ic_dot_n");
    rs_ic_dot[(size_t) Direction::W] = ps::res::idx("ic_dot_w");
    rs_ic_dot[(size_t) Direction::E] = ps::res::idx("ic_dot_e");
    rs_ic_dot[(size_t) Direction::S] = ps::res::idx("ic_dot_s");

    rs_ic_pin[(size_t) Direction::N] = ps::res::idx("ic_pin_n");
    rs_ic_pin[(size_t) Direction::W] = ps::res::idx("ic_pin_w");
    rs_ic_pin[(size_t) Direction::E] = ps::res::idx("ic_pin_e");
    rs_ic_pin[(size_t) Direction::S] = ps::res::idx("ic_pin_s");
}
