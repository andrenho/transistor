#include "ui.hh"

#include <pl_log.h>

#include "resources/images/bg.jpg.h"
#include "resources/images/circuit.png.h"
#include "resources/images/circuit.tileset.lua.h"
#include "resources/images/infobox.png.h"
#include "resources/images/infobox.tileset.lua.h"
#include "resources/fonts/CutePixel.ttf.h"
#include "resources/fonts/Minecraftia.ttf.h"

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
    ps::res::add_image("infobox", ui_resources_images_infobox_png, ui_resources_images_infobox_png_sz);
    ps::res::add_tiles_from_lua("infobox", ui_resources_images_infobox_tileset_lua, ui_resources_images_infobox_tileset_lua_sz);
    ps::res::add_ttf("font", ui_resources_fonts_Minecraftia_ttf, ui_resources_fonts_Minecraftia_ttf_sz);
    ps::res::add_ttf("ic_font", ui_resources_fonts_CutePixel_ttf, ui_resources_fonts_CutePixel_ttf_sz);

    SDL_Event resize = { .type = SDL_EVENT_WINDOW_RESIZED };
    SDL_PushEvent(&resize);

    gui.init();
}

bool UI::running() const
{
    ps::graphics::timestep();
    return ps::graphics::running();
}

std::vector<luaobj::Event> UI::get_user_events() const
{
    std::vector<luaobj::Event> events;

    auto button_name = [](Uint8 button) -> std::string {
        switch (button) {
            case 1: return "left";
            case 2: return "middle";
            case 3: return "right";
            default: return "unknown";
        }
    };

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT || (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_Q))
            ps::graphics::quit();

        luaobj::Event::KeyMod mod = {
#if SDL_PLATFORM_APPLE
            .ctrl = (e.key.mod & SDL_KMOD_GUI) != 0,
#else
            .ctrl = (e.key.mod & SDL_KMOD_CTRL) != 0,
#endif
            .alt = (e.key.mod & SDL_KMOD_ALT) != 0,
            .shift = (e.key.mod & SDL_KMOD_SHIFT) != 0
        };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
        switch (e.type) {
            case SDL_EVENT_MOUSE_MOTION:
                events.push_back({ .type = "move_pointer", .x = (int) e.motion.x, .y = (int) e.motion.y });
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                events.push_back({ .type = "mouse_press", .button = button_name(e.button.button) });
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                events.push_back({ .type = "mouse_release", .button = button_name(e.button.button), .index = e.button.button });
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                events.push_back({ .type = "mouse_wheel", .y = e.wheel.y });
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.repeat == 0)
                    events.push_back({ .type = "key_down", .key = std::string(1, (char) e.key.key), .mod = mod, .index = e.button.button });
                break;
            case SDL_EVENT_KEY_UP:
                events.push_back({ .type = "key_up", .key = std::string(1, (char) e.key.key), .mod = mod });
                break;
            case SDL_EVENT_WINDOW_RESIZED: {
                int w, h;
                SDL_GetWindowSize(ps::graphics::window(), &w, &h);
                events.push_back({ .type = "window_resize", .x = w, .y = h });
                break;
            }
            default: break;
        }
#pragma GCC diagnostic pop

        gui.do_event(&e);
    }

    return events;
}

void UI::do_events(std::vector<luaobj::Event> const& events)
{
    for (auto const& event: events)
        gui.do_event(event);
}

std::vector<luaobj::Event> UI::render_and_get_gui_events(luaobj::Render const& render, Engine& engine) const
{
    std::vector scenes = { background_scene() };
    scenes.insert(scenes.end(), std::make_move_iterator(render.scenes.begin()), std::make_move_iterator(render.scenes.end()));
    ps::graphics::render_scenes(scenes);

    auto events = gui.render(render, engine);

    ps::graphics::set_window_title(std::format("transistor ({} FPS -- {}K steps/sec)", ps::graphics::fps(), simulation_steps_ / 1000));

    ps::graphics::present();

    return events;
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
