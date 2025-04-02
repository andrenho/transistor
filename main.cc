#include <pl_log.h>

#include "luaenv/lua.hh"
#include "luaenv/hotreload.hh"
#include "mappers/engine.hh"

#include "ui/ui.hh"

int main()
{
    pl_init();

    Lua lua;
    HotReload hotreload(lua);
    Engine engine(lua);

    size_t i = 0;
    UI ui;
    while (ui.running()) {
        if (hotreload.restart())
            engine.setup();

        auto events = ui.events();
        engine.events(events);

        // TODO - update from engine

        auto render = engine.render();
        events = ui.render(render, engine);
        if (!events.empty())
            engine.events(events);

        if (i % (60 * 3) == 0)
            engine.save_in_progress();
        ++i;
    }

    engine.save_in_progress();
}
