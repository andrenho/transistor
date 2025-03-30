#ifndef DIALOG__HH
#define DIALOG__HH

#include "mappers/engine.hh"
#include "mappers/in/render.hh"

void render_dialogs(std::vector<luaobj::Dialog const*> const& dialogs, Engine& engine);

#endif //DIALOG__HH
