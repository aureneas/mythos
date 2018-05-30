#pragma once

#include <mythos\utility\matrix.h>


#define __MYTHOS_VERY_SMALL 0.0001


struct MythosLineSegment {

	vec3f		origin;
	vec3f		dir;
};

struct MythosPlane {

	vec3f		normal;
	float		coeff;

	int			inBounds(vec3f&);

};