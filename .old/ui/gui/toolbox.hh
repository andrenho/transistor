#ifndef TOOLBOX_HH
#define TOOLBOX_HH

extern "C" {
#include <transistor-sandbox.h>
}

void toolbox_init();
void toolbox_render(ts_Transistor* T);

#endif //TOOLBOX_HH
