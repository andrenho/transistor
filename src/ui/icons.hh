#ifndef ICONS_HH
#define ICONS_HH
#include "res/resourcemanager.hh"

inline void load_icons()
{
    res().add_tiles("circuit", {
        { "tile", 2, 2 },
        { "board_top_left", 0, 0, 2, 2 },
        { "board_top", 2, 0, 1, 2 },
        { "board_top_right", 3, 0, 2, 2 },
        { "board_left", 0, 2, 2, 1 },
        { "board_right", 3, 2, 2, 1 },
        { "board_bottom_left", 0, 3, 2, 2 },
        { "board_bottom", 2, 3, 1, 2 },
        { "board_bottom_right", 3, 3, 2, 2 },
        { "vcc", 8, 2 },
        { "npn", 5, 2 },
        { "pnp", 5, 3 },
        { "button_off", 6, 2 },
        { "button_on", 6, 3 },
        { "led_off", 7, 2 },
        { "lef_on", 7, 3 },
        { "shadow_rect", 5, 4 },
        { "shadow_square", 6, 4 },
        { "shadow_circle", 7, 4 },
        { "wire_top_on_north_1", 0, 7 },
        { "wire_top_on_east_1", 1, 7 },
        { "wire_top_on_west_1", 2, 7 },
        { "wire_top_on_south_1", 3, 7 },
        { "wire_top_off_north_1", 0, 5 },
        { "wire_top_off_east_1", 1, 5 },
        { "wire_top_off_west_1", 2, 5 },
        { "wire_top_off_south_1", 3, 5 },
        { "arrow", 5, 1 },
    }, 16);
}

#endif //ICONS_HH
