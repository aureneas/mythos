#ifndef MYTHOS_POINT_H
#define MYTHOS_POINT_H


struct Point {
    int x;
    int y;

    void operator+=(const Point&);
    int operator*(const Point&);
};


#endif // MYTHOS_POINT_H
