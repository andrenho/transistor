#include <mappers/engine.hh>

#include "luaenv/array.hh"
#include "luaenv/lua.hh"
#include "luaenv/hotreload.hh"

int main()
{
    Lua lua;
    HotReload hotreload(lua);

    lua.with_lua([](lua_State* L) { setup_array(L); });

    Engine engine(lua);
}