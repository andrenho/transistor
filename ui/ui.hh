#ifndef UI_HH
#define UI_HH

#include <pastel2d.hh>
#include <vector>

#include "gui/gui.hh"
#include "mappers/engine.hh"
#include "mappers/out/event.hh"
#include "mappers/in/render.hh"

class UI {
public:
    UI();

    [[nodiscard]] bool                       running() const;
    [[nodiscard]] std::vector<luaobj::Event> events() const;
    void                                     render(luaobj::Render const& render, Engine& engine) const;

private:
    ps::res::idx_t rs_bg = -1;
    GUI gui;

    ps::Scene background_scene() const;
};

#endif //UI_HH
