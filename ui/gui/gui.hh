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

    void init(int circuit_tx_w, int circuit_tx_h);
    void do_event(SDL_Event* e) const;

    std::vector<luaobj::Event> render(luaobj::Render const& render, Engine& engine) const;

private:
    ImGuiIO* io = nullptr;
    int circuit_tx_w_, circuit_tx_h_;
    mutable bool show_demo_window_ = true;
    mutable bool show_error_window_ = false;

    static void setup_theme();
    void setup_font();
    void error_window(luaobj::Render const& render, Engine const& engine) const;

    static std::vector<luaobj::Dialog const*> dialog_list(luaobj::Render const& render) ;
};

#endif //GUI_HH
