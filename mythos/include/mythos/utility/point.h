#ifndef MYTHOS_POINT_H
#define MYTHOS_POINT_H


struct Vec2 {
    int x;
    int y;

    void operator+=(const Vec2&);
    void operator*=(int);

    int operator*(const Vec2&);
};

typedef Vec2 Point;

struct Vec3 {
    int x, y, z;

    void operator+=(const Vec3&);
    void operator*=(int);

    int operator*(const Vec3&);
    int operator*(const Vec2&);
};


#endif // MYTHOS_POINT_H
