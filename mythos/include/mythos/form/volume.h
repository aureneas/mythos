#ifndef MYTHOS_VOLUME_H
#define MYTHOS_VOLUME_H

#include <list>
#include <vector>

#include "../point.h"


enum Direction : int {
    EAST = 0,
    NORTHEAST = 1,
    NORTH = 2,
    NORTHWEST = 3,
    WEST = 4,
    SOUTHWEST = 5,
    SOUTH = 6,
    SOUTHEAST = 7
};

Point direction_to_point(Direction);


typedef std::vector<Point> VertexList;

struct Polygon {
    Point center;
    VertexList vertex;

    Polygon();
    Polygon(Polygon*);
    Polygon adjust(Point);
    bool intersect(Polygon*);
};

struct Volume {
    Polygon base;
    int z;
    int height;

    Volume();
    Volume(Volume*);
    Volume adjust(Point, int);
    bool intersect(Volume*);
};


#endif // MYTHOS_VOLUME_H
