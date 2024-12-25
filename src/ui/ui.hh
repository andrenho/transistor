#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include "uiinterface.hh"
#include "ui_layer.hh"
#include "engine/sandbox/sandbox.hh"
#include "resources/resourcemanager.hh"

using hr = std::chrono::high_resolution_clock;
using Duration = decltype(hr::now() - hr::now());

class UI : public UI_Interface {
public:
    UI();
    ~UI();
    UI (const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void set_sandbox(Sandbox& sandbox);

    void draw_image(UILayer const* layer, Resource const& res, int x, int y, DrawProperties const& dp) const override;
    void start_dragging(UILayer* layer) override;
    void stop_dragging() override;

    void update([[maybe_unused]] Duration timestep);
    void render() const;

    [[nodiscard]] bool running() const { return running_; }

private:
    void init_imgui();

    void                                        drag_layer(UILayer* layer, int xrel, int yrel);
    std::tuple<UILayer*, int, int>              find_layer(int x, int y) const;
    std::vector<std::tuple<UILayer*, int, int>> find_all_layers(int x, int y) const;

    bool running_ = true;

    SDL_Window*   window_;
    SDL_Renderer* ren_;
    SDL_Cursor    *move_cursor_;

    ResourceManager resource_manager_;
    Resource        bg_;

    std::vector<std::unique_ptr<UILayer>> layers;
    std::optional<UILayer*> dragging_;

    mutable Duration frame_time_ = std::chrono::milliseconds(0);
    mutable size_t   frame_count_ = 0;
};

#endif
