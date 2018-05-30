#pragma once

#include "matrix.h"

struct Line {

	vec3f origin;
	vec3f direction;

	vec3f getPoint(float);
};

struct Plane {

	vec3f normal;
	float coeff;

	float getIntersect(const Line&);
};