#pragma once

#include "matrix.h"

class MYTHOS_CORE_API Line {

private:
	mat4f mMatrix;

public:
	constexpr Line() {}
	Line(const mat4f&);
	Line(const vec3f&, const vec3f&);
	
	constexpr vec3f getOrigin(void);
	constexpr vec3f getDirection(void);
	constexpr vec3f getPoint(float);

	constexpr Line transform(const mat4f&);
};

struct MYTHOS_CORE_API Plane {

	vec3f normal;
	float coeff;

	float getIntersect(const Line&);
};