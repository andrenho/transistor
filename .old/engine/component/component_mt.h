#ifndef COMPONENT_MT_H
#define COMPONENT_MT_H

#include <lua.h>

typedef struct ts_Component ts_Component;
typedef struct LuaComponentData { ts_Component* component; } LuaComponentData;

void ts_components_mt_init(lua_State* L);

#endif //COMPONENT_MT_H
