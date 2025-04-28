#include <simulator/simulator.hh>

#include "luaenv/lua.hh"
#include "luaenv/hotreload.hh"
#include "mappers/engine.hh"

#include "ui/ui.hh"

int main()
{
    Lua lua;
    HotReload hotreload(lua);
    Engine engine(lua);
    Simulator simulator(lua);

    size_t i = 0;
    UI ui;
    while (ui.running()) {

        // restart engine?
        if (hotreload.restart())
            engine.setup();

        // process user events
        auto events = ui.get_user_events();
        if (auto compiled_circuit = engine.do_events(events))
            simulator.update_compiled_circuit(std::move(*compiled_circuit));

        // render, and process GUI events
        auto render = engine.render(simulator.wires_values());
        events = ui.render_and_get_gui_events(render, engine);
        if (!events.empty()) {
            ui.do_events(events);
            engine.do_events(events);
        }

        // timed operations
        if (i % 60 == 0)
            ui.set_simulation_steps(simulator.steps());
        if (i % (60 * 3) == 0)
            engine.save_in_progress();
        ++i;
    }

    engine.save_in_progress();
}
