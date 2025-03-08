#ifndef COMMON_H
#define COMMON_H

#include <transistor-sandbox.h>

extern char common_savename[];
extern char common_savename_file[];

void common_quit(ts_Transistor* T);
void common_unstash_work(ts_Transistor* T);
void common_clear(ts_Transistor* T);
void common_save(ts_Transistor* T);
void common_save_as(ts_Transistor* T, const char* filename);
void common_load(ts_Transistor* T, const char* filename);

#endif //COMMON_H
