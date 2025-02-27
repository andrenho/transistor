#include <stdio.h>

#include <SDL3/SDL.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include "transistor-sandbox.h"

#include "board.h"
#include "gui.h"
#include "pastel2d.h"
#include "resources.h"

#define MAX_SCENES 64

static size_t background_scene(ps_Scene* scenes, size_t n_scenes);

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

            board_update(&T, &snap, &e);
            gui_events(&e);
        }

        // create and render scenes

        size_t n_scenes = 0;
        static ps_Scene scenes[MAX_SCENES];

        n_scenes = background_scene(scenes, n_scenes);
        n_scenes = board_create_scenes(&snap, scenes, n_scenes);

        ts_snapshot_finalize(&snap);
        if (n_scenes >= MAX_SCENES)
            abort();
        ps_graphics_render_scenes(scenes, n_scenes);

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

size_t background_scene(ps_Scene* scenes, size_t n_scenes)
{
    ps_Scene* scene = &scenes[n_scenes++];
    scene->z_order = 1000;
    ps_scene_init(scene);

    int ww, wh, rw, rh;
    SDL_GetWindowSize(ps_graphics_window(), &ww, &wh);
    ps_res_image_size(rs_bg, &rw, &rh);
    ps_scene_add_image(scenes, rs_bg, (SDL_Rect) { ww/2 - rw/2, wh/2 - rh/2 }, NULL);

    return n_scenes;
}
