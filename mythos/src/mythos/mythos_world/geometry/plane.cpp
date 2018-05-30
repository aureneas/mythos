#include <mythos_world\geometry\plane.h>

int MythosPlane::inBounds(vec3f& pos) {

	return (abs(pos.dot(normal) - coeff) < __MYTHOS_VERY_SMALL);
}