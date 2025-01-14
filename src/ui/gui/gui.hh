#ifndef GUI_HH
#define GUI_HH

#include <string>

#include "engine/game/game.hh"
#include "SDL2/SDL.h"

class GUI {
public:
    void init(SDL_Window* window, SDL_Renderer* ren);
    void shutdown();

    void process_events(SDL_Event* e);

    bool render(Game const& game, SDL_Renderer* ren);

    void set_modal_exception(std::optional<std::string> const& modal_exception) { modal_exception_ = modal_exception; }

private:
    void setup_theme();

    bool render_main_menu(Game const& game);
    void render_infobox();
    void render_toolbox();
    bool render_modal_exception();

    struct ImGuiIO* io = nullptr;
    bool            show_demo_window_ = false;

    std::optional<std::string> modal_exception_ {};
};

#endif //GUI_HH
