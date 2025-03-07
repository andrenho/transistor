#include "pos_ds.h"

#include <stb_ds.h>
#include <stdbool.h>

bool arrcontains(ts_Position* haystack, ts_Position needle)
{
    for (int i = 0; i < arrlen(haystack); ++i)
        if (ts_pos_equals(haystack[i], needle))
            return true;
    return false;
}
