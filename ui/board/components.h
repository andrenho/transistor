#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <transistor-sandbox.h>
#include <pastel2d.h>

// initialization
int G_init(lua_State* L);

// rendering
void component_renderer_setup(ts_Transistor* T, ps_Scene* scene);
void component_render(ts_Transistor* T, ts_ComponentSnapshot const* component);

#endif //COMPONENTS_H
