#include "ui.hh"

#include <iostream>
#include <string>

#include "icons.hh"
#include "res/resourcemanager.hh"
#include "util/visitor.hh"
using namespace std::string_literals;

#include "scene.hh"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include "devices/board/boardeditor.hh"

#include "battery/embed.hpp"

#include "devices/deviceeditor.hh"

UI::UI()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_SetHint (SDL_HINT_RENDER_VSYNC, "1");

    window_ = SDL_CreateWindow(PROJECT_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 800, SDL_WINDOW_RESIZABLE);
    if (!window_)
        throw std::runtime_error("Error: SDL_CreateWindow(): "s + SDL_GetError());

    ren_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!ren_)
        throw std::runtime_error("Error: SDL_CreateRenderer(): "s + SDL_GetError());

    SDL_RendererInfo info;
    SDL_GetRendererInfo(ren_, &info);
    SDL_Log("Current SDL_Renderer: %s", info.name);

    res().add_texture<"resources/images/bg.jpg">("bg");
    res().add_texture<"resources/images/circuit.png">("icons");
    load_icons();

    gui_.init(window_, ren_, icons_);

    move_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    delete_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    recreate_devices();
}

UI::~UI()
{
    gui_.shutdown();

    res().cleanup();

    SDL_FreeCursor(delete_cursor_);
    SDL_FreeCursor(move_cursor_);
    if (ren_)
        SDL_DestroyRenderer(ren_);
    if (window_)
        SDL_DestroyWindow(window_);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void UI::recreate_devices()
{
    device_editors_.clear();
    for (auto const& board: game().sandbox().boards())
        device_editors_.push_back(std::make_unique<BoardEditor>(board.id()));
}

void UI::operator<<(U::Command&& command) const
{
    commands_.emplace(command);
}

void UI::execute_queue()
{
    while (!commands_.empty()) {

        auto command = std::move(commands_.front());
        commands_.pop();

        std::visit(overloaded {
            [&](U::StartDragginDevice const& cmd) {
                dragging_ = cmd.device_editor;
                SDL_SetCursor(move_cursor_);
            },
            [&](U::StopDraggingDevice const&) {
                dragging_ = {};
                SDL_SetCursor(SDL_GetDefaultCursor());
            },
            [&](U::SetMouseCursor const& cmd) {
                switch (cmd.cursor) {
                    case U::SetMouseCursor::Normal:
                        SDL_SetCursor(SDL_GetDefaultCursor());
                        break;
                    case U::SetMouseCursor::Delete:
                        SDL_SetCursor(delete_cursor_);
                        break;
                    default: break;
                }
            },
            [&](U::Quit const&) {
                running_ = false;
            },
            [&](U::UpdateToolbox const& cmd) {
                state_.toolbox = cmd.toolbox;
            },
            [&](U::ShowException const& cmd) {
                state_.exception = cmd;
            },
            [&](U::ClearException const&) {
                state_.exception = {};
            },
            [&](U::SetInfobox const& cmd) {
                state_.infobox_contents = cmd.contents;
            }
        }, command);
    }
}

void UI::update(Duration timestep)
{
    ++frame_count_;
    ++frame_time_ += timestep;
    ++total_frames_;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {

        gui_.process_events(&e);

        int mx, my;

        switch (e.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (auto [layer, lx, ly] = find_device(e.button.x, e.button.y); layer)
                    layer->on_mouse_press(lx, ly, e.button.button, e.button.clicks == 2);
                break;
            case SDL_MOUSEBUTTONUP:
                for (auto [layer, lx, ly] : find_all_devices(e.button.x, e.button.y))
                    layer->on_mouse_release(lx, ly, e.button.button);
                break;
            case SDL_MOUSEMOTION:
                if (auto [layer, lx, ly] = find_device(e.button.x, e.button.y); layer)
                    layer->on_mouse_move(lx, ly, e.motion.xrel, e.motion.yrel);
                if (dragging_)
                    drag_device(*dragging_, e.motion.xrel, e.motion.yrel);
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat == 0) {
                    SDL_GetMouseState(&mx, &my);
                    if (auto [layer, lx, ly] = find_device(mx, my); layer)
                        layer->on_key_press(e.key.keysym.sym, lx, ly);
#ifndef NODEBUG
                    if (e.key.keysym.sym == SDLK_q)
                        running_ = false;
#endif
                }
                break;
            case SDL_KEYUP:
                SDL_GetMouseState(&mx, &my);
                for (auto [layer, lx, ly] : find_all_devices(mx, my))
                    layer->on_key_release(e.key.keysym.sym, lx, ly);
                break;
            default: break;
        }
    }

    execute_queue();
}

void UI::draw_image(Scene::Image const& image, DeviceEditor const* layer) const
{
    SDL_Texture* texture = nullptr;
    SDL_Rect origin;

    auto resource = res().get(image.resource);

    if (auto tx = std::get_if<SDL_Texture*>(&resource)) {
        texture = *tx;
        origin.x = origin.y = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &origin.w, &origin.h);
    } else if (auto tile = std::get_if<Tile>(&resource)) {
        texture = tile->texture;
        origin = { tile->x, tile->y, tile->w, tile->h };
    } else {
        throw std::runtime_error("Invalid resource.");
    }

    const SDL_Rect dest = {
        .x = (int) (layer->x() / layer->zoom()) + image.x,
        .y = (int) (layer->y() / layer->zoom()) + image.y,
        .w = origin.w,
        .h = origin.h
    };

    SDL_SetTextureAlphaMod(texture, image.pen.semitransparent ? 128 : 255);

    SDL_RenderSetScale(ren_, layer->zoom(), layer->zoom());
    SDL_RenderCopyEx(ren_, texture, &origin, &dest, dir_angle_clockwise(image.pen.rotation), nullptr, SDL_FLIP_NONE);
    SDL_RenderSetScale(ren_, 1.f, 1.f);
}

void UI::render() const
{
    // clear screen
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren_);

    // draw background
    SDL_RenderCopy(ren_, res().texture("bg"), nullptr, nullptr);

    // draw layers
    for (auto& layer: device_editors_) {
        Scene scene;
        layer->render(scene);
        while (auto image = scene.next_image()) {
            draw_image(*image, layer.get());
        }
    }

    // draw gui
    if (!gui_.render(ren_, state_))
        *this << U::Quit {};

    // present to screen
    SDL_RenderPresent(ren_);

    // show FPS
    if (frame_count_ > 20) {
        int fps = 1000 / (std::chrono::duration_cast<std::chrono::milliseconds>(frame_time_).count() / frame_count_);
        SDL_SetWindowTitle(window_, (std::string(PROJECT_NAME) + " (" + std::to_string(fps) + " FPS)").c_str());
        frame_time_ = std::chrono::milliseconds(0);
        frame_count_ = 0;
    }
}

void UI::drag_device(DeviceEditor* layer, int xrel, int yrel)
{
    int scr_w, scr_h;
    SDL_GetWindowSize(window_, &scr_w, &scr_h);

    int min_x = (-layer->w() + 56) * layer->zoom();
    int min_y = (-layer->h() + 56) * layer->zoom();
    int max_x = scr_w - 56 * layer->zoom();
    int max_y = scr_h - 56 * layer->zoom();

    layer->set_x(std::min(std::max(layer->x() + xrel, min_x), max_x));
    layer->set_y(std::min(std::max(layer->y() + yrel, min_y), max_y));
}

std::tuple<DeviceEditor*, int, int> UI::find_device(int x, int y) const
{
    for (auto const& layer: device_editors_) {
        if (x >= layer->x() && y >= layer->y()
                && x < (layer->x() + (layer->w() * layer->zoom()))
                && y < (layer->y() + (layer->h() * layer->zoom()))) {
            return { layer.get(), (x - layer->x()) / layer->zoom(), (y - layer->y()) / layer->zoom() };
        }
    }
    return { nullptr, 0, 0 };
}

std::vector<std::tuple<DeviceEditor*, int, int>> UI::find_all_devices(int x, int y) const
{
    std::vector<std::tuple<DeviceEditor*, int, int>> r;
    r.reserve(device_editors_.size());
    for (auto const& layer: device_editors_)
        r.emplace_back(layer.get(), (x - layer->x()) / layer->zoom(), (y - layer->y()) / layer->zoom());
    return r;
}

UI const& ui()
{
    static const UI ui;
    return ui;
}

void ui_update(Duration timestep)
{
    const_cast<UI*>(&ui())->update(timestep);
}