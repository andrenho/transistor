#include "transistor.hh"

#include <pl_log.h>
#include <pastel2d.hh>

#include "images/bg.jpg.h"

static ps::res::idx_t rs_bg;

[[noreturn]] static void error_callback(void* _)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", pl_last_error(), ps_graphics_window());
    abort();
}

static ps::Scene background_scene()
{
    ps::Scene scene;
    scene.set_z_order(1000);

    int ww, wh;
    SDL_GetWindowSize(ps_graphics_window(), &ww, &wh);
    auto [rw, rh] = ps::res::image_size(rs_bg);
    scene.add_image(rs_bg, (SDL_Rect) { ww/2 - rw/2, wh/2 - rh/2, 0, 0 });
    return scene;
}

int main(int argc, char* argv[])
{
    pl_init();

    // initialize engine and run tests
    Transistor T;
    if (argc == 2 && strcmp(argv[1], "-t") == 0)
        return EXIT_SUCCESS;

    // initialize graphics
    PL_INFO("pastel2d version %s", ps::version().c_str());
    ps::init({
        .appname = "transistor",
        .appidentifier = "com.github.andrenho.transistor",
        .appversion = PROJECT_VERSION,
        .window_w = 1400,
        .window_h = 900,
        .flags = SDL_WINDOW_RESIZABLE,
    });
    pl_set_error_callback(error_callback, nullptr);
    pl_set_abort_callback(error_callback, nullptr);
    ps::graphics::set_bg(20, 40, 60);

    // load bg
    rs_bg = ps::res::add_image(frontend_resources_images_bg_jpg, frontend_resources_images_bg_jpg_sz);

    while (ps::graphics::running()) {
        ps::graphics::timestep();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
                ps::graphics::quit();
            // TODO - handle events
        }

        std::vector<ps::Scene> scenes = { background_scene() };
        // TODO - render graphics from engine
        ps::graphics::render_scenes(scenes);

        // TODO - ps::graphics::set_window_title(std::format("transistor ({} FPS -- {}K steps/sec)", ps::graphics::fps(), steps_per_second / 1000));
        ps::graphics::present();
    }
}
