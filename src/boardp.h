#ifndef BOARDP_H
#define BOARDP_H

#include "scene.h"
#include "transistor-sandbox.h"

void boardp_update(ts_Transistor* T, ts_TransistorSnapshot const* snap, SDL_Event* e);
void boardp_create_scene(ts_TransistorSnapshot const* snap, ps_Scene* scene);

#endif //BOARDP_H