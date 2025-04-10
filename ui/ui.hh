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
    std::vector<luaobj::Event>               render(luaobj::Render const& render, Engine& engine) const;

    void set_simulation_steps(uint64_t simulation_steps) { simulation_steps_ = simulation_steps; }

private:
    ps::res::idx_t rs_bg = -1;
    GUI            gui;
    uint64_t       simulation_steps_ = 0;

    ps::Scene background_scene() const;
};

#endif //UI_HH
