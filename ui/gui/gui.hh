#ifndef GUI_HH
#define GUI_HH

#include <imgui.h>
#include <SDL3/SDL.h>

#include "mappers/engine.hh"
#include "mappers/in/render.hh"

class GUI {
public:
    GUI();
    ~GUI();

    void init();
    void do_event(SDL_Event* e) const;

    void render(Render const& render, Engine& engine) const;

private:
    ImGuiIO* io = nullptr;
    mutable bool show_demo_window_ = true;
    mutable bool show_error_window_ = false;

    static void setup_theme();
    void setup_font();
    void error_window(Render const& render, Engine const& engine) const;
};

#endif //GUI_HH
