#include "common.h"

#include <libgen.h>
#include <sys/stat.h>

#include <pastel2d.h>
#include <pl_log.h>
#include <stdlib.h>

#include <transistor-sandbox.h>

#include "board/components.h"

#define AUTOSAVE_ENABLED 0

char common_savename[1024] = "";
char common_savename_file[1024] = "";

static const char* stash_name()
{
    static char filename[1024] = "";
    if (filename[0] == '\0') {
        const char* home = getenv("HOME");
        snprintf(filename, sizeof filename, "%s/.transistor/stash.trn", home);
    }
    return filename;
}

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


void common_stash_work(ts_Transistor* T)
{
#if AUTOSAVE_ENABLED
    char* bpath = strdup(stash_name());
    char* path = dirname(bpath);
    mkdir(path, 0700);
    free(bpath);

    FILE* f = fopen(stash_name(), "w");
    if (f) {
        ts_serialize_to_file(T, f);
        fclose(f);
        PL_DEBUG("Stash saved to '%s'.", stash_name());
    } else {
        PL_ERROR("Cloud not open stash file '%s'.", stash_name());
    }
#endif
}

void common_unstash_work(ts_Transistor* T)
{
#if AUTOSAVE_ENABLED
    PL_DEBUG("Loading stash from %s.", stash_name());
    FILE* f = fopen(stash_name(), "r");
    if (f) {
        ts_TransistorConfig config = ts_config(T);
        ts_finalize(T);
        ts_unserialize_from_file(T, config, f, G_init);
        fclose(f);
        PL_DEBUG("Stash loaded.");
    } else {
        PL_INFO("Stash file not present.");
    }
#endif
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
    ts_init(T, config, G_init);
}

void common_save(ts_Transistor* T)
{
    if (common_savename[0] == '\0')
        return;
    FILE* f = fopen(common_savename, "w");
    if (f) {
        ts_serialize_to_file(T, f);
        fclose(f);
        PL_INFO("Circuit saved as '%s'.", common_savename);
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
        ts_unserialize_from_file(T, config, f, G_init);
        fclose(f);
        common_set_savename(filename);
        PL_INFO("Circuit loaded from '%s'.", filename);
    } else {
        PL_ERROR("Cloud not open file '%s'.", common_savename);
    }
}