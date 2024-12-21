#ifndef UI_UI_HH_
#define UI_UI_HH_

#include <chrono>

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
    void load_resources();
    void init_imgui();
    void render_game();
    void render_gui();

    bool running_ = true;
    bool show_demo_window_ = true;

    struct SDL_Window*   window_;
    struct SDL_Renderer* ren_;
    struct SDL_Texture*  texture_;
    struct ImGuiIO* io;
};

#endif
