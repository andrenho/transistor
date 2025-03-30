#ifndef MENU_HH
#define MENU_HH

#include "mappers/engine.hh"
#include "mappers/in/render.hh"

void render_menu(luaobj::Render const& render, std::vector<luaobj::Event>& events);

#endif //MENU_HH
