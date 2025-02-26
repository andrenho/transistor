#include <stdio.h>

#include <SDL3/SDL.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include "transistor-sandbox.h"

#include "boardp.h"
#include "pastel2d.h"
#include "resources.h"

static void print_versions()
{
    const int linked = SDL_GetVersion();
    SDL_Log("pastel2d version %s", ps_version(NULL, NULL, NULL));
    SDL_Log("transistor-sandbox version %s", ts_transistor_version(NULL, NULL, NULL));
}

static void update(size_t timestep_us, ts_Transistor* t, ts_TransistorSnapshot* snap)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
            ps_graphics_quit();

        boardp_update(t, snap, &e);
    }
}

static ps_Scene* create_scenes(size_t* n_scenes, ts_TransistorSnapshot* snap)
{
    *n_scenes = snap->n_boards;
    ps_Scene* scenes = calloc(*n_scenes, sizeof(ps_Scene));

    for (size_t i = 0; i < snap->n_boards; ++i)
        boardp_create_scene(snap, &scenes[i]);

    return scenes;
}

int main(void)
{
    print_versions();

    ts_Transistor T;
    ts_transistor_init(&T, (ts_TransistorConfig) { false, false });

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

    while (ps_graphics_running()) {

        ts_TransistorSnapshot snap;
        ts_transistor_take_snapshot(&T, &snap);

        update(ps_graphics_timestep_us(), &T, &snap);

        size_t n_scenes;
        ps_Scene* scenes = create_scenes(&n_scenes, &snap);
        ps_graphics_render_scenes(scenes, n_scenes);
        free(scenes);

        ts_snapshot_finalize(&snap);

        ps_graphics_present();
    }

    ps_finalize();

    ts_transistor_finalize(&T);

    return 0;
}
