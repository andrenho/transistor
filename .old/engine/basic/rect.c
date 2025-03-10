#include "rect.h"

bool ts_rect_a_inside_b(ts_Rect a, ts_Rect b)
{
    return a.top_left.x >= b.top_left.x && a.top_left.y >= b.top_left.y &&
           a.bottom_right.x <= b.bottom_right.x && a.bottom_right.y <= b.bottom_right.y;
}