#ifndef RECT_HH
#define RECT_HH

#include "position.hh"

bool overlap(Position const& p, Position const& r1, Position const& r2);
bool overlap(Position const& p1, Position const& p2, Position const& r1, Position const& r2);

#endif //RECT_HH
