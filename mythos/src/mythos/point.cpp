#include "../../include/mythos/point.h"

void Point::operator+=(Point p) {
    x += p.x;
    y += p.y;
}
