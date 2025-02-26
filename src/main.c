#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#include <SDL3/SDL.h>

#include "pastel2d.h"
#include "transistor-sandbox.h"

ts_Transistor T;

static void print_versions()
{
    const int linked = SDL_GetVersion();
    SDL_Log("pastel2d version %s", ps_version(NULL, NULL, NULL));
    SDL_Log("transistor-sandbox version %s", ts_transistor_version(NULL, NULL, NULL));
}

static void update(size_t timestep_us)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
        if (e.type == SDL_EVENT_QUIT)
            ps_graphics_quit();
}

static ps_Scene* create_scenes(size_t* n_scenes)
{
    *n_scenes = 0;
    return NULL;
}

int main(void)
{
    print_versions();

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

    while (ps_graphics_running()) {
        update(ps_graphics_timestep_us());

        size_t n_scenes;
        ps_Scene* scenes = create_scenes(&n_scenes);
        ps_graphics_render_scenes(scenes, n_scenes);
        free(scenes);

        ps_graphics_present();
    }

    ps_finalize();

    ts_transistor_finalize(&T);

    return 0;
}
