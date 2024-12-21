#include "ui/ui.hh"

#include <chrono>

#include "lua/luaengine.hh"

int main()
{
    LuaEngine lua_engine;
    lua_engine.hello();

    UI ui;
    auto last_frame = hr::now();
    while (ui.running()) {
        auto new_frame = hr::now();
        ui.update(new_frame - last_frame);
        ui.render();
        last_frame = new_frame;
    }
}
