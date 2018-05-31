#include <mythos\utility\geometry.h>


Line::Line(const mat4f& matrix) {

	mMatrix = matrix;
}

Line::Line(const vec3f& origin, const vec3f& direction) {

	for (int i = 2; i >= 0; --i) {
		mMatrix(i, i) = direction.getValue(i);
		mMatrix(i, 3) = origin.getValue(i);
	}
}

constexpr vec3f Line::getOrigin(void) {

	return getPoint(0.0f);
}

constexpr vec3f Line::getDirection(void) {

	return vec3f(mMatrix % vec4f(1.0f, 1.0f, 1.0f, 0.0f));
}

constexpr vec3f Line::getPoint(float t) {

	return vec3f(mMatrix % vec4f(t, t, t));
}

Line Line::transform(const mat4f& matrix) {

	return Line(matrix % mMatrix);
}


float Plane::getIntersect(const Line& line) {

	float nDotD = normal.dot(line.getDirection());

	if (nDotD == 0.0f)
		return INFINITY;

	return (coeff - normal.dot(line.getOrigin())) / nDotD;
}