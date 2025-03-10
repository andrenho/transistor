#ifndef RECT_H
#define RECT_H
#include "position.h"

typedef struct ts_Rect {
    ts_Position top_left;
    ts_Position bottom_right;
} ts_Rect;

bool ts_rect_a_inside_b(ts_Rect a, ts_Rect b);

#endif //RECT_H
