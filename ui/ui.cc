#include "ui.hh"

#include <pl_log.h>

#include "resources/images/bg.jpg.h"
#include "resources/images/circuit.png.h"
#include "resources/images/circuit.tileset.lua.h"

[[noreturn]] static void error_callback(void* _)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", pl_last_error(), ps_graphics_window());
    abort();
}

UI::UI()
{
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

    // load resources
    rs_bg = ps::res::add_image(ui_resources_images_bg_jpg, ui_resources_images_bg_jpg_sz);
    ps::res::add_image("circuit", ui_resources_images_circuit_png, ui_resources_images_circuit_png_sz);
    ps::res::add_tiles_from_lua("circuit", ui_resources_images_circuit_tileset_lua, ui_resources_images_circuit_tileset_lua_sz);

    gui.init();
}

bool UI::running() const
{
    ps::graphics::timestep();
    return ps::graphics::running();
}

std::vector<luaobj::Event> UI::events() const
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
            ps::graphics::quit();
        gui.do_event(&e);
    }
    return {};  // TODO
}

void UI::render(luaobj::Render const& render, Engine& engine) const
{
    std::vector scenes = { background_scene() };
    scenes.insert(scenes.end(), std::make_move_iterator(render.scenes.begin()), std::make_move_iterator(render.scenes.end()));
    ps::graphics::render_scenes(scenes);

    gui.render(render, engine);

    ps::graphics::set_window_title(std::format("transistor ({} FPS -- {}K steps/sec)", ps::graphics::fps(), 0 / 1000));

    ps::graphics::present();
}

ps::Scene UI::background_scene() const
{
    ps::Scene scene;
    scene.set_z_order(1000);

    int ww, wh;
    SDL_GetWindowSize(ps_graphics_window(), &ww, &wh);
    auto [rw, rh] = ps::res::image_size(rs_bg);
    scene.add_image(rs_bg, (SDL_Rect) { ww/2 - rw/2, wh/2 - rh/2, 0, 0 });
    return scene;
}
