#include "rect.hh"

bool overlap(Position const& p1, Position const& p2, Position const& r1, Position const& r2)
{
    // Calculate the corners of first rectangle
    int left1 = std::min(p1.x, p2.x);
    int right1 = std::max(p1.x, p2.x);
    int top1 = std::max(p1.y, p2.y);
    int bottom1 = std::min(p1.y, p2.y);

    // Calculate the corners of second rectangle
    int left2 = std::min(r1.x, r2.x);
    int right2 = std::max(r1.x, r2.x);
    int top2 = std::max(r1.y, r2.y);
    int bottom2 = std::min(r1.y, r2.y);

    // Check if one rectangle is on the left side of the other
    if (right1 < left2 || right2 < left1)
        return false;

    // Check if one rectangle is above the other
    if (bottom1 > top2 || bottom2 > top1)
        return false;

    return true;
}
