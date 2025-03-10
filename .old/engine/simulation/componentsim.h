#ifndef COMPONENTSIM_HH
#define COMPONENTSIM_HH

#include "component/componentdef.h"
#include "util/result.h"

ts_Result ts_component_sim_init(ts_Sandbox* sandbox);
ts_Result ts_component_sim_prepare(lua_State* L);
ts_Result ts_component_sim_add_component(lua_State* L, ts_Component* component, int idx);
ts_Result ts_component_sim_execute(ts_Sandbox const* sandbox);

#endif //COMPONENTSIM_HH
