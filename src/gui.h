#ifndef GUI_H
#define GUI_H

#include <SDL3/SDL_events.h>

void gui_init();
void gui_events(SDL_Event* e);
void gui_render();
void gui_finalize();

#endif //GUI_H
