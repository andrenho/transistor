#include <pl_log.h>

#include "luaenv/array.hh"
#include "luaenv/lua.hh"
#include "luaenv/hotreload.hh"
#include "mappers/engine.hh"
#include "mappers/in/render.hh"
#include "mappers/out/event.hh"

#include "ui/ui.hh"

int main()
{
    pl_init();

    Lua lua;
    HotReload hotreload(lua);

    lua.with_lua([](lua_State* L) { setup_array(L); });
    Engine engine(lua);

    UI ui;
    while (ui.running()) {
        auto events = ui.events();
        engine.events(events);

        // TODO - update from engine

        auto render = engine.render();
        events = ui.render(render, engine);
        if (!events.empty())
            engine.events(events);
    }
}