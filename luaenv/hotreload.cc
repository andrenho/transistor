#include "hotreload.hh"

#include "dmon.h"

HotReload::HotReload(Lua& lua)
{
#ifdef DEV
    lua.with_lua([](lua_State* L) {
        lua_pushcfunction(L, [](lua_State* L) -> int {
            const char* name = luaL_checkstring(L, 1);

            lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
            lua_pushnil(L);
            lua_setfield(L, -2, name);
            lua_pop(L, 1);

            lua_getglobal(L, "package");
            lua_getfield(L, -1, "loaded");
            lua_pushnil(L);
            lua_setfield(L, -2, name);
            lua_pop(L, 2);

            return 0;
        });
        lua_setglobal(L, "unload_package");
    });

    // watch for files and restart simulation
    dmon_init();
    dmon_watch("./engine",
        [](dmon_watch_id watch_id, dmon_action action, const char* dirname, const char* filename, const char* oldname, void* user) {
            if (std::string(filename).ends_with(".tl")) {
                bool* restart = (bool *) user;
                *restart = true;
            }
        }, DMON_WATCHFLAGS_RECURSIVE | DMON_WATCHFLAGS_FOLLOW_SYMLINKS, &restart_);
#endif
}

HotReload::~HotReload()
{
#ifdef DEV
    dmon_deinit();
#endif
}
