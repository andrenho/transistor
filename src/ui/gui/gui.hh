#ifndef GUI_HH
#define GUI_HH

#include "SDL2/SDL.h"

class GUI {
public:
    void init(SDL_Window* window, SDL_Renderer* ren);
    void shutdown();

    void process_events(SDL_Event* e);
    void render(SDL_Renderer* ren);

private:
    void setup_theme();

    struct ImGuiIO* io = nullptr;
    bool show_demo_window_ = true;
};

#endif //GUI_HH
