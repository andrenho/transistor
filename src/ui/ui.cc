#include "ui.hh"

#include <string>

#include "layers/boardeditor.hh"
using namespace std::string_literals;

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

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

UI::~UI()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    resource_manager_.cleanup();

    if (ren_)
        SDL_DestroyRenderer(ren_);
    if (window_)
        SDL_DestroyWindow(window_);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
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
                break;
            case SDL_KEYDOWN:
                SDL_GetMouseState(&mx, &my);
                if (auto [layer, lx, ly] = find_layer(mx, my); layer)
                    layer->on_key_press(*this, e.key.keysym.sym, lx, ly);
                break;
#ifndef NODEBUG
                if (e.key.keysym.sym == SDLK_q)
                    running_ = false;
#endif
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

    SDL_Rect dest = { layer->pos_x + x, layer->pos_y + y, origin.w, origin.h };

    SDL_RenderCopy(ren_, texture, &origin, &dest);
}

void UI::render()
{
    // clear screen
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren_);

    // draw background
    SDL_RenderCopy(ren_, bg_, nullptr, nullptr);

    // draw layers
    for (auto& layer: layers) {
        SDL_RenderSetScale(ren_, layer->zoom, layer->zoom);
        layer->render(*(UI_Interface const*) this);
        SDL_RenderSetScale(ren_, 1.f, 1.f);
    }

    SDL_RenderPresent(ren_);
}

std::tuple<UILayer*, int, int> UI::find_layer(int x, int y) const
{
    for (auto const& layer: layers) {
        if (x >= layer->pos_x && y >= layer->pos_y
                && x < (layer->pos_x + (layer->w * layer->zoom))
                && y < (layer->pos_y + (layer->h * layer->zoom))) {
            return { layer.get(), (x - layer->pos_x) / layer->zoom, (y - layer->pos_y) / layer->zoom };
        }
    }
    return { nullptr, 0, 0 };
}
