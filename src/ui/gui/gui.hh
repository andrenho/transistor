#ifndef GUI_HH
#define GUI_HH

#include "engine/game/game.hh"
#include "SDL2/SDL.h"

class GUI {
public:
    void init(SDL_Window* window, SDL_Renderer* ren);
    void shutdown();

    void set_game(Game& game) { game_ = &game; }

    void process_events(SDL_Event* e);
    bool render(SDL_Renderer* ren);

private:
    void setup_theme();

    bool main_menu();

    struct ImGuiIO* io = nullptr;
    Game*           game_;
    bool            show_demo_window_ = false;
};

#endif //GUI_HH
