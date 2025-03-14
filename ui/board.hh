#ifndef BOARD_HH
#define BOARD_HH

#include <SDL3/SDL_events.h>
#include <pastel2d.hh>

#include "engine.hh"

void      board_events(Engine& engine, SDL_Event const* event, Snapshot::Board const& board);
ps::Scene board_scene(Snapshot::Board const& board);

#endif //BOARD_HH
