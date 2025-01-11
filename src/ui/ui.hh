#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "scene.hh"
#include "SDL2/SDL.h"

#include "layers/layer.hh"
#include "engine/sandbox/sandbox.hh"
#include "gui/gui.hh"
#include "resources/resourcemanager.hh"

using hr = std::chrono::high_resolution_clock;
using Duration = decltype(hr::now() - hr::now());

class UI {
public:
    UI();
    ~UI();
    UI (const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void set_sandbox(Sandbox& sandbox);

    void update([[maybe_unused]] Duration timestep);
    void render();

    [[nodiscard]] bool running() const { return running_; }

private:
    void                                      draw_image(Scene::Image const& image, Layer const* layer) const;
    void                                      do_events(Events const& events);

    void                                      drag_layer(Layer* layer, int xrel, int yrel);
    std::tuple<Layer*, int, int>              find_layer(int x, int y) const;
    std::vector<std::tuple<Layer*, int, int>> find_all_layers(int x, int y) const;

    bool running_ = true;

    SDL_Window*   window_;
    SDL_Renderer* ren_;
    SDL_Cursor    *move_cursor_, *delete_cursor_;
    std::vector<Resource> icons_;

    GUI             gui_;
    ResourceManager resource_manager_;
    Resource        bg_;

    std::vector<std::unique_ptr<Layer>> layers_;
    std::optional<Layer*> dragging_;

    mutable Duration frame_time_ = std::chrono::milliseconds(0);
    mutable size_t   frame_count_ = 0;
};

#endif
