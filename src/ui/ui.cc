#include "ui.hh"

#include <string>

#include "layers/boardeditor.hh"
using namespace std::string_literals;

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "battery/embed.hpp"

#include "ui_layer.hh"

UI::UI()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

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
                    layer->on_mouse_press(*this, lx, ly, e.button.button, e.button.clicks == 2);
                break;
            case SDL_MOUSEBUTTONUP:
                if (auto [layer, lx, ly] = find_layer(e.button.x, e.button.y); layer)
                    layer->on_mouse_release(*this, lx, ly, e.button.button);
                break;
            case SDL_MOUSEMOTION:
                if (auto [layer, lx, ly] = find_layer(e.motion.x, e.motion.y); layer)
                    layer->on_mouse_move(*this, lx, ly, e.motion.xrel, e.motion.yrel);
                if (dragging_)
                    drag_layer(*dragging_, e.motion.xrel, e.motion.yrel);
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat == 0) {
                    SDL_GetMouseState(&mx, &my);
                    if (auto [layer, lx, ly] = find_layer(mx, my); layer)
                        layer->on_key_press(*this, e.key.keysym.sym, lx, ly);
#ifndef NODEBUG
                    if (e.key.keysym.sym == SDLK_q)
                        running_ = false;
#endif
                }
                break;
            case SDL_KEYUP:
                SDL_GetMouseState(&mx, &my);
                if (auto [layer, lx, ly] = find_layer(mx, my); layer)
                    layer->on_key_release(*this, e.key.keysym.sym, lx, ly);
                break;
            default: break;
        }
    }
}

void UI::draw_image(UILayer const* layer, Resource const& res, int x, int y, DrawProperties const& dp) const
{
    SDL_Texture* texture;
    SDL_Rect origin;

    if (res.is_texture()) {
        texture = res;
        origin.x = origin.y = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &origin.w, &origin.h);
    } else {
        Resource::SubTexture st = res;
        texture = st.texture;
        origin = { st.x, st.y, st.w, st.h };
    }

    const SDL_Rect dest = {
        .x = (int) (layer->x() / layer->zoom()) + x,
        .y = (int) (layer->y() / layer->zoom()) + y,
        .w = origin.w,
        .h = origin.h
    };

    SDL_SetTextureAlphaMod(texture, dp.semitransparent ? 128 : 255);
    SDL_RenderCopy(ren_, texture, &origin, &dest);
}

void UI::start_dragging(UILayer* layer)
{
    dragging_ = layer;
    SDL_SetCursor(move_cursor_);
}

void UI::stop_dragging()
{
    dragging_ = {};
    SDL_SetCursor(SDL_GetDefaultCursor());
}

void UI::render() const
{
    // clear screen
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren_);

    // draw background
    SDL_RenderCopy(ren_, bg_, nullptr, nullptr);

    // draw layers
    for (auto& layer: layers) {
        SDL_RenderSetScale(ren_, layer->zoom(), layer->zoom());
        layer->render(*this);
        SDL_RenderSetScale(ren_, 1.f, 1.f);
    }

    SDL_RenderPresent(ren_);
}

void UI::drag_layer(UILayer* layer, int xrel, int yrel)
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

std::tuple<UILayer*, int, int> UI::find_layer(int x, int y) const
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
