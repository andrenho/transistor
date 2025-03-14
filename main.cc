#include <pl_log.h>
#include <pastel2d.hh>
#include <ui/board.hh>
#include <ui/lua_interface.hh>
#include <ui/resources.hh>

#include "engine.hh"
#include "tests.hh"

[[noreturn]] static void error_callback(void* _)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", pl_last_error(), ps_graphics_window());
    abort();
}

ps::Scene background_scene()
{
    ps::Scene scene;
    scene.set_z_order(1000);

    int ww, wh;
    SDL_GetWindowSize(ps_graphics_window(), &ww, &wh);
    auto [rw, rh] = ps::res::image_size(rs_bg);
    scene.add_image(rs_bg, (SDL_Rect) { ww/2 - rw/2, wh/2 - rh/2, 0, 0 });
    return scene;
}

int main()
{
    pl_init();
    Tests::run_tests();

    // initialize engine
    Engine engine;
    engine.with_lua_object(initialize_lua_G_object);
    engine.start();

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

    // initialize resources
    load_resources();

    // TODO initialize GUI

    //
    // main loop
    //

    while (ps::graphics::running()) {
        Snapshot snapshot = engine.take_snapshot();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
                ps::graphics::quit();
            for (auto const& board: snapshot.boards)
                board_events(engine, &e, board);
            // TODO - gui events
        }

        // create and render scenes

        std::vector<ps::Scene> scenes;
        scenes.push_back(background_scene());
        for (auto const& board: snapshot.boards)
            scenes.push_back(board_scene(engine, board));
        ps::graphics::render_scenes(scenes);

        // present display

        ps_graphics_present();
    }
}
