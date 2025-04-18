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
    void do_event(luaobj::Event const& event);

    std::vector<luaobj::Event> render(luaobj::Render const& render, Engine& engine) const;

private:
    ImGuiIO* io = nullptr;
    mutable bool show_demo_window_ = false;
    mutable bool show_error_window_ = false;

    static void setup_theme();
    void setup_font();
    void error_window(luaobj::Render const& render, Engine const& engine) const;

    static std::vector<luaobj::Dialog const*> dialog_list(luaobj::Render const& render);
    void execute_ui_events(std::vector<luaobj::Event> const& vector) const;
};

#endif //GUI_HH
