#include "../../include/mythos/point.h"

void Point::operator+=(const Point& p) {
    x += p.x;
    y += p.y;
}

int Point::operator*(const Point& p) {
    return (x * p.x) + (y * p.y);
}
