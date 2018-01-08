#include <cmath>
#include "../../../include/mythos/form/volume.h"

Point direction_to_point(Direction d) {
    double theta = (int)d * 3.14159 / 180;
    double ct = cos(theta);
    double st = sin(theta);
    return { (ct > 0 ? 1 : (ct < 0 ? -1 : 0)), (st > 0 ? 1 : (st < 0 ? -1 : 0)) };
}


bool Volume::intersect(Volume* v) {
	// TODO implement intersection algorithm
	return false;
}
