#include "common.h"

#include <pastel2d.h>

char common_savename[1024] = "";

static void common_stash_work()
{
}

void common_unstash_work()
{

}

void common_quit()
{
    common_stash_work();
    ps_graphics_quit();
}

void common_clear(ts_Transistor* T)
{
    common_savename[0] = '\0';
    ts_TransistorConfig config = ts_config(T);
    ts_finalize(T);
    ts_init(T, config);
}

void common_save(ts_Transistor* T)
{
    if (common_savename[0] == '\0')
        return;
    // TODO
}

void common_save_as(ts_Transistor* T, const char* filename)
{
    strcpy(common_savename, filename);
    common_save(T);
}

void common_load(ts_Transistor* T, const char* filename)
{

}