#ifndef LUA_INTERFACE_HH
#define LUA_INTERFACE_HH

#include <lua.hpp>
#include <pastel2d.hh>

extern int G_luaref;

void initialize_lua_G_object(lua_State* L);
void G_set_scene(lua_State* L, ps::Scene* scene);

#endif //LUA_INTERFACE_HH
