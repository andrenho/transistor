#include <pl_log.h>
#include <simulator/simulator.hh>

#include "luaenv/lua.hh"
#include "luaenv/hotreload.hh"
#include "mappers/engine.hh"
#include "mappers/in/compiled_circuit.hh"

#include "ui/ui.hh"

int main()
{
    pl_init();

    Lua lua;
    HotReload hotreload(lua);
    Engine engine(lua);
    Simulator simulator(lua);

    size_t i = 0;
    UI ui;
    while (ui.running()) {
        if (hotreload.restart())
            engine.setup();

        auto events = ui.events();
        if (auto compiled_circuit = engine.events(events))
            simulator.update_compiled_circuit(std::move(*compiled_circuit));

        auto render = engine.render(simulator.wires_values());
        events = ui.render(render, engine);
        if (!events.empty())
            engine.events(events);

        if (i % 60 == 0)
            ui.set_simulation_steps(simulator.steps());
        if (i % (60 * 3) == 0)
            engine.save_in_progress();
        ++i;
    }

    engine.save_in_progress();
}
