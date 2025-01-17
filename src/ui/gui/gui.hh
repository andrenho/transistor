#ifndef GUI_HH
#define GUI_HH

#include "toolbox.hh"
#include "SDL2/SDL.h"

class GUI {
public:
    void init(SDL_Window* window, SDL_Renderer* ren);
    void shutdown();

    void process_events(SDL_Event* e);

    bool render() const;

private:
    void setup_theme();

    bool render_main_menu() const;
    void render_infobox() const;
    bool render_modal_exception() const;

    struct ImGuiIO* io = nullptr;
    mutable bool    show_demo_window_ = false;
    Toolbox         toolbox_;
};

#endif //GUI_HH
