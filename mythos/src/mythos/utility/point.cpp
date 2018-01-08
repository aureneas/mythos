#include "../../../include/mythos/utility/point.h"

void Point::operator+=(const Point& p) {
    x += p.x;
    y += p.y;
}

void Point::operator*=(int i) {
    x *= i;
    y *= i;
}

int Point::operator*(const Point& p) {
    return (x * p.x) + (y * p.y);
}



void Vec3::operator+=(const Vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vec3::operator*=(int i) {
    x *= i;
    y *= i;
    z *= i;
}

int Vec3::operator*(const Vec3& v) {
    return (x * v.x) + (y * v.y) + (z * v.z);
}

int Vec3::operator*(const Vec2& v) {
    return (x * v.x) + (y * v.y);
}
