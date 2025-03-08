#ifndef COMMON_H
#define COMMON_H

#include <transistor-sandbox.h>

extern char common_savename[];

void common_quit();
void common_unstash_work();
void common_clear(ts_Transistor* T);
void common_save(ts_Transistor* T);
void common_save_as(ts_Transistor* T, const char* filename);
void common_load(ts_Transistor* T, const char* filename);

#endif //COMMON_H
