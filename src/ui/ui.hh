#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>

#include "resources/resourcemanager.hh"

using hr = std::chrono::high_resolution_clock;
using Duration = decltype(hr::now() - hr::now());

class UI {
public:
    UI();
    ~UI();
    UI (const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void update([[maybe_unused]] Duration timestep);
    void render();

    [[nodiscard]] bool running() const { return running_; }

private:
    void init_imgui();
    void render_gui();

    bool running_ = true;
    bool show_demo_window_ = true;

    struct SDL_Window*   window_;
    struct SDL_Renderer* ren_;
    struct ImGuiIO* io;

    ResourceManager resource_manager_;
    Resource        bg_, circuit_;
};

#endif
