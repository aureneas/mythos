#include <cmath>
#include "../../../include/mythos/form/volume.h"

Point direction_to_point(Direction d) {
    double theta = (int)d * 3.14159 / 180;
    double ct = cos(theta);
    double st = sin(theta);
    return { (ct > 0 ? 1 : (ct < 0 ? -1 : 0)), (st > 0 ? 1 : (st < 0 ? -1 : 0)) };
}


Polygon::Polygon(Polygon* p) {
    center = p->center;
    for (std::vector<Point>::iterator it = p->vertex.begin(); it != p->vertex.end(); ++it)
        vertex.push_back(*it);
}

Polygon Polygon::adjust(Point p) {
    Polygon poly(this);
    poly.center += p;
    for (std::vector<Point>::iterator it = poly.vertex.begin(); it != poly.vertex.end(); ++it)
        *it += p;
    return poly;
}

bool Polygon::intersect(Polygon* p) {
    // TODO later
    return false;
}

Volume::Volume(Volume* v) : base(&v->base) {
    z = v->z;
    height = v->height;
}

Volume Volume::adjust(Point p, int dz) {
    Volume v(this);
    v.base = v.base.adjust(p);
    v.z += dz;
    return v;
}

bool Volume::intersect(Volume* v) {
    if (base.intersect(&v->base))
        return ((z + height > v->z && z <= v->z) || (v->z + v->height > z && v->z <= z));
    return false;
}
