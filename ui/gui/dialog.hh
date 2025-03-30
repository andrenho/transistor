#ifndef DIALOG__HH
#define DIALOG__HH

#include "mappers/engine.hh"
#include "mappers/in/render.hh"

void render_dialogs(std::vector<luaobj::Dialog const*> const& dialogs, std::vector<luaobj::Event>& events);

#endif //DIALOG__HH
