#include <mythos\utility\geometry.h>

vec3f Line::getPoint(float t) {

	return origin + (direction * t);
}

float Plane::getIntersect(const Line& line) {

	float nDotD = normal.dot(line.direction);

	if (nDotD == 0.0f)
		return INFINITY;

	return (coeff - normal.dot(line.origin)) / nDotD;
}