#include "ui.hh"

#include <string>

#include "util/visitor.hh"
using namespace std::string_literals;

#include "scene.hh"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include "layers/board/boardeditor.hh"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "battery/embed.hpp"

#include "layers/layer.hh"

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

    resource_manager_.set_renderer(ren_);
    bg_ = resource_manager_.from_image(b::embed<"resources/images/bg.jpg">().vec());

    init_imgui();

    move_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
}

UI::~UI()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    resource_manager_.cleanup();

    SDL_FreeCursor(move_cursor_);
    if (ren_)
        SDL_DestroyRenderer(ren_);
    if (window_)
        SDL_DestroyWindow(window_);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void UI::set_sandbox(Sandbox& sandbox)
{
    Board& board = *sandbox.editor().boards().begin();
    layers.push_back(std::make_unique<BoardEditor>(resource_manager_, board));
}

void UI::init_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO* io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window_, ren_);
    ImGui_ImplSDLRenderer2_Init(ren_);
}

void UI::update(Duration timestep)
{
    ++frame_count_;
    ++frame_time_ += timestep;

    Events events;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {

        ImGui_ImplSDL2_ProcessEvent(&e);
        int mx, my;

        switch (e.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (auto [layer, lx, ly] = find_layer(e.button.x, e.button.y); layer)
                    layer->on_mouse_press(lx, ly, e.button.button, e.button.clicks == 2, events);
                break;
            case SDL_MOUSEBUTTONUP:
                for (auto [layer, lx, ly] : find_all_layers(e.button.x, e.button.y))
                    layer->on_mouse_release(lx, ly, e.button.button, events);
                break;
            case SDL_MOUSEMOTION:
                if (auto [layer, lx, ly] = find_layer(e.button.x, e.button.y); layer)
                    layer->on_mouse_move(lx, ly, e.motion.xrel, e.motion.yrel, events);
                if (dragging_)
                    drag_layer(*dragging_, e.motion.xrel, e.motion.yrel);
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat == 0) {
                    SDL_GetMouseState(&mx, &my);
                    if (auto [layer, lx, ly] = find_layer(mx, my); layer)
                        layer->on_key_press(e.key.keysym.sym, lx, ly, events);
#ifndef NODEBUG
                    if (e.key.keysym.sym == SDLK_q)
                        running_ = false;
#endif
                }
                break;
            case SDL_KEYUP:
                SDL_GetMouseState(&mx, &my);
                for (auto [layer, lx, ly] : find_all_layers(mx, my))
                    layer->on_key_release(e.key.keysym.sym, lx, ly, events);
                break;
            default: break;
        }
    }

    do_events(events);
}

void UI::draw_image(Scene::Image const& image) const
{
    SDL_Texture* texture;
    SDL_Rect origin;

    if (image.resource.is_texture()) {
        texture = image.resource;
        origin.x = origin.y = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &origin.w, &origin.h);
    } else {
        Resource::SubTexture st = image.resource;
        texture = st.texture;
        origin = { st.x, st.y, st.w, st.h };
    }

    const SDL_Rect dest = {
        .x = (int) (image.context->x() / image.context->zoom()) + image.x,
        .y = (int) (image.context->y() / image.context->zoom()) + image.y,
        .w = origin.w,
        .h = origin.h
    };

    SDL_SetTextureAlphaMod(texture, image.pen.semitransparent ? 128 : 255);

    SDL_RenderSetScale(ren_, image.context->zoom(), image.context->zoom());
    SDL_RenderCopy(ren_, texture, &origin, &dest);
    SDL_RenderSetScale(ren_, 1.f, 1.f);
}

void UI::do_events(Events const& events)
{
    for (auto const& event: events) {
        std::visit(overloaded {
            [&](event::StartDragging const& sd) {
                dragging_ = sd.layer;
                SDL_SetCursor(move_cursor_);
            },
            [&](event::StopDragging const&) {
                dragging_ = {};
                SDL_SetCursor(SDL_GetDefaultCursor());
            }
        }, event);
    }
}

void UI::render() const
{
    // create scene
    Scene scene;
    scene.bg = bg_;
    for (auto& layer: layers)
        layer->render(scene);

    // clear screen
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren_);

    // draw background
    SDL_RenderCopy(ren_, scene.bg, nullptr, nullptr);

    // draw images
    for (auto const& image: scene.images())
        draw_image(image);
    SDL_RenderPresent(ren_);

    // show FPS
    if (frame_count_ > 20) {
        int fps = 1000 / (std::chrono::duration_cast<std::chrono::milliseconds>(frame_time_).count() / frame_count_);
        SDL_SetWindowTitle(window_, (std::string(PROJECT_NAME) + " (" + std::to_string(fps) + " FPS)").c_str());
        frame_time_ = std::chrono::milliseconds(0);
        frame_count_ = 0;
    }
}

void UI::drag_layer(Layer* layer, int xrel, int yrel)
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

std::tuple<Layer*, int, int> UI::find_layer(int x, int y) const
{
    for (auto const& layer: layers) {
        if (x >= layer->x() && y >= layer->y()
                && x < (layer->x() + (layer->w() * layer->zoom()))
                && y < (layer->y() + (layer->h() * layer->zoom()))) {
            return { layer.get(), (x - layer->x()) / layer->zoom(), (y - layer->y()) / layer->zoom() };
        }
    }
    return { nullptr, 0, 0 };
}

std::vector<std::tuple<Layer*, int, int>> UI::find_all_layers(int x, int y) const
{
    std::vector<std::tuple<Layer*, int, int>> r;
    r.reserve(layers.size());
    for (auto const& layer: layers)
        r.emplace_back(layer.get(), (x - layer->x()) / layer->zoom(), (y - layer->y()) / layer->zoom());
    return r;
}
