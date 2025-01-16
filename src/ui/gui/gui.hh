#ifndef GUI_HH
#define GUI_HH

#include <string>

#include "engine/game/game.hh"
#include "SDL2/SDL.h"
#include "ui/uistate.hh"

class GUI {
public:
    void init(SDL_Window* window, SDL_Renderer* ren);
    void shutdown();

    void process_events(SDL_Event* e);

    bool render(SDL_Renderer* ren, UIState const& state) const;

private:
    void setup_theme();

    bool render_main_menu() const;
    void render_infobox(UIState const& state) const;
    void render_toolbox(UIState const& state) const;
    bool render_modal_exception(UIState const& state) const;

    struct ImGuiIO* io = nullptr;
    mutable bool    show_demo_window_ = false;
};

#endif //GUI_HH
