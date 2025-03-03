#ifndef BOARDP_H
#define BOARDP_H

#include <pastel2d.h>
#include <transistor-sandbox.h>

void   board_update(ts_Transistor* T, ts_TransistorSnapshot const* snap, SDL_Event* e);
size_t board_create_scenes(ts_Transistor const* t, ts_TransistorSnapshot const* snap, ps_Scene* scenes, size_t start);

#endif //BOARDP_H