#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#include <SDL3/SDL.h>

#include "pastel2d.h"
#include "transistor-sandbox.h"

static void print_versions()
{
    const int linked = SDL_GetVersion();
    SDL_Log("SDL version %d.%d.%d", SDL_VERSIONNUM_MAJOR(linked), SDL_VERSIONNUM_MINOR(linked), SDL_VERSIONNUM_MICRO(linked));
    SDL_Log("pastel2d version %s", ps_version(NULL, NULL, NULL));
    SDL_Log("transistor-sandbox version %s", ts_transistor_version(NULL, NULL, NULL));
}

int main(void)
{
    print_versions();

    PS_ASSERT(ps_init(&(ps_GraphicsInit) {
        .appname = "transistor",
        .appidentifier = "com.github.andrenho.transistor",
        .appversion = PROJECT_VERSION,
        .window_w = 1400,
        .window_h = 900,
        .flags = SDL_WINDOW_RESIZABLE,
    }));

    while (ps_graphics_running()) {
        ps_graphics_timestep_us();

        SDL_Event e;
        while (SDL_PollEvent(&e))
            if (e.type == SDL_EVENT_QUIT)
                ps_graphics_quit();

        ps_graphics_present();
    }

    ps_finalize();

    return 0;
}
