#include <stdio.h>

#include <SDL3/SDL.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconstant-logical-operand"
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#pragma clang diagnostic pop

#define PL_LOG_IMPLEMENTATION
#include <pl_log.h>

#include <pastel2d.h>
#include <transistor-sandbox.h>

#include "board/board.h"
#include "board/components.h"
#include "gui/gui.hh"
#include "resources.h"

#define MAX_SCENES 64

static size_t background_scene(ps_Scene* scenes, size_t n_scenes);

[[noreturn]] static void error_callback(void* _)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", pl_last_error(), ps_graphics_window());
    abort();
}

int main(void)
{
    pl_init();
    pl_set_error_callback(error_callback, NULL);
    pl_set_abort_callback(error_callback, NULL);
    PL_INFO("pastel2d version %s", ps_version());
    PL_INFO("transistor-sandbox version %s", ts_version());

    // initialize sandbox

    ts_Transistor T;
    // ts_init(&T, (ts_TransistorConfig) { false, false });
    ts_init(&T, (ts_TransistorConfig) { true, false });

    // initialize graphics

    if (ps_init(&(ps_GraphicsInit) {
        .appname = "transistor",
        .appidentifier = "com.github.andrenho.transistor",
        .appversion = PROJECT_VERSION,
        .window_w = 1400,
        .window_h = 900,
        .flags = SDL_WINDOW_RESIZABLE,
    }) != 0) {
        fprintf(stderr, "Error initializing graphics: %s", pl_last_error());
        exit(EXIT_FAILURE);
    }

    ps_graphics_set_bg(20, 40, 60);
    gui_init();

    // initialize resources

    load_resources();
    components_init(&T);

    //
    // main loop
    //

    while (ps_graphics_running()) {

        ps_graphics_timestep_us();

        // process sandbox

        ts_run(&T, 6000);

        // do events

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
                ps_graphics_quit();

            board_update(&T, &e);
            gui_events(&e);
        }

        // create and render scenes

        size_t n_scenes = 0;
        static ps_Scene scenes[MAX_SCENES];

        n_scenes = background_scene(scenes, n_scenes);

        ts_TransistorSnapshot snap;
        ts_take_snapshot(&T, &snap);
        n_scenes = board_create_scenes(&T, &snap, scenes, n_scenes);
        ts_snapshot_finalize(&snap);

        if (n_scenes >= MAX_SCENES)
            PL_ABORT();
        ps_graphics_render_scenes(scenes, n_scenes);

        gui_render();

        // present display

        ps_graphics_set_window_title("transistor (%d FPS -- %dK steps/sec)", ps_graphics_fps(), ts_steps_per_second(&T) / 1000);
        ps_graphics_present();
    }

    // finalize

    gui_finalize();
    ps_finalize();
    ts_finalize(&T);
    pl_close();

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
    ps_scene_add_image(scenes, rs_bg, (SDL_Rect) { ww/2 - rw/2, wh/2 - rh/2, 0, 0 }, NULL);

    return n_scenes;
}
