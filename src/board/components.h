#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <transistor-sandbox.h>
#include <pastel2d.h>

void components_init(ts_Transistor* t);
void component_render(ts_Transistor const* T, ts_ComponentSnapshot const* component, ps_Scene* scene);

#endif //COMPONENTS_H
