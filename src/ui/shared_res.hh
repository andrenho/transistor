#ifndef SHAREDICONS_HH
#define SHAREDICONS_HH

inline void load_shared_resources()
{
    constexpr int TILE_SIZE = 16;

    res().add_tiles("icons", {
        { "shadow_rect", 5, 4 },
        { "shadow_square", 6, 4 },
        { "shadow_circle", 7, 4 },
        { "button_off", 6, 2 },
        { "button_on", 6, 3 },
        { "led_off", 7, 2 },
        { "led_on", 7, 3 },
        { "vcc", 8, 2 },
        { "npn", 5, 2 },
        { "pnp", 5, 3 },
    }, TILE_SIZE);
}

#endif //SHAREDICONS_HH
