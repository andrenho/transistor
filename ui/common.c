#include "common.h"

#include <libgen.h>
#include <sys/stat.h>

#include <pastel2d.h>
#include <pl_log.h>
#include <stdlib.h>

#include "board/components.h"

char common_savename[1024] = "";
char common_savename_file[1024] = "";

const char* stash_name = "˜/.transistor/stash";

static void common_set_savename(const char* filename)
{
    strcpy(common_savename, filename);
    if (common_savename[0] != '\0') {
        char* bname = strdup(common_savename);
        char* fname = basename(bname);
        snprintf(common_savename_file, sizeof common_savename_file, "%s", fname);
        free(bname);
    } else {
        common_savename_file[0] = '\0';
    }
}


static void common_stash_work(ts_Transistor* T)
{
    mkdir("˜/.transistor", 0700);
    FILE* f = fopen(stash_name, "w");
    if (f) {
        ts_serialize_to_file(T, f);
        fclose(f);
    } else {
        PL_ERROR("Cloud not open stash file '%s'.", stash_name);
    }
}

void common_unstash_work(ts_Transistor* T)
{
    FILE* f = fopen(stash_name, "r");
    if (f) {
        ts_TransistorConfig config = ts_config(T);
        ts_finalize(T);
        ts_unserialize_from_file(T, config, f);
        components_init(T);
        fclose(f);
    }
}

void common_quit(ts_Transistor* T)
{
    common_stash_work(T);
    ps_graphics_quit();
}

void common_clear(ts_Transistor* T)
{
    common_set_savename("");
    ts_TransistorConfig config = ts_config(T);
    ts_finalize(T);
    ts_init(T, config);
    components_init(T);
}

void common_save(ts_Transistor* T)
{
    if (common_savename[0] == '\0')
        return;
    FILE* f = fopen(common_savename, "w");
    if (f) {
        ts_serialize_to_file(T, f);
        fclose(f);
    } else {
        PL_ERROR("Cloud not open file '%s'.", common_savename);
    }
}

void common_save_as(ts_Transistor* T, const char* filename)
{
    common_set_savename(filename);
    common_save(T);
}

void common_load(ts_Transistor* T, const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f) {
        ts_TransistorConfig config = ts_config(T);
        ts_finalize(T);
        ts_unserialize_from_file(T, config, f);
        components_init(T);
        fclose(f);
        common_set_savename(filename);
    } else {
        PL_ERROR("Cloud not open file '%s'.", common_savename);
    }
}