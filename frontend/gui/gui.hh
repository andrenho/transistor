#ifndef GUI_HH
#define GUI_HH

#include <imgui.h>
#include <SDL3/SDL.h>

#include "render.hh"

class GUI {
public:
    GUI();
    ~GUI();

    void init();
    void do_event(SDL_Event* e);

    void render(Render const& render);

private:
    ImGuiIO* io = nullptr;
    bool show_demo_window_ = true;
    bool show_error_window_ = false;

    static void setup_theme();
    void setup_font();
    void error_window(Render const& render);
};

#endif //GUI_HH
