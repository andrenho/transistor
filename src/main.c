#include <stdio.h>

#include <SDL3/SDL.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include "transistor-sandbox.h"

#include "boardp.h"
#include "gui.h"
#include "pastel2d.h"
#include "resources.h"

int main(void)
{
    SDL_Log("pastel2d version %s", ps_version(NULL, NULL, NULL));
    SDL_Log("transistor-sandbox version %s", ts_transistor_version(NULL, NULL, NULL));

    // initialize sandbox

    ts_Transistor T;
    ts_transistor_init(&T, (ts_TransistorConfig) { false, false });

    // initialize graphics

    PS_ASSERT(ps_init(&(ps_GraphicsInit) {
        .appname = "transistor",
        .appidentifier = "com.github.andrenho.transistor",
        .appversion = PROJECT_VERSION,
        .window_w = 1400,
        .window_h = 900,
        .flags = SDL_WINDOW_RESIZABLE,
    }));
    ps_graphics_set_bg(93, 135, 161);

    load_resources();

    gui_init();

    //
    // main loop
    //

    while (ps_graphics_running()) {

        ps_graphics_timestep_us();

        // process sandbox

        ts_transistor_run(&T, 6000);

        ts_TransistorSnapshot snap;
        ts_transistor_take_snapshot(&T, &snap);

        // do events

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
                ps_graphics_quit();

            boardp_update(&T, &snap, &e);
            gui_events(&e);
        }

        // create and render scenes

        ps_Scene* scenes;
        size_t n_scenes = boardp_create_scenes(&snap, &scenes);
        ts_snapshot_finalize(&snap);
        ps_graphics_render_scenes(scenes, n_scenes);
        free(scenes);

        gui_render();

        // present display

        ps_graphics_present();
    }

    // finalize

    gui_finalize();
    ps_finalize();
    ts_transistor_finalize(&T);

    return 0;
}
