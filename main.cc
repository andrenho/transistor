#include "luaenv/array.hh"
#include "luaenv/lua.hh"
#include "luaenv/hotreload.hh"
#include "mappers/engine.hh"
#include "mappers/in/render.hh"
#include "mappers/out/event.hh"

#include "ui/ui.hh"

int main()
{
    Lua lua;
    HotReload hotreload(lua);

    lua.with_lua([](lua_State* L) { setup_array(L); });
    Engine engine(lua);

    UI ui;
    while (ui.running()) {
        std::vector<Event> events = ui.events();
        engine.events(events);
        // TODO - update from engine

        Render render = engine.render();
        ui.render(render);
    }
}