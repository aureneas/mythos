#ifndef MYTHOS_VOLUME_H
#define MYTHOS_VOLUME_H

#include <list>
#include <vector>

#include "../utility/point.h"


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


struct Volume {
	Vec3	crd;
	Vec3	dim;

	virtual bool intersect(Volume*);	// volume intersection
};


#endif // MYTHOS_VOLUME_H
