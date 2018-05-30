#include <mythos_world\geometry\shape.h>



MythosCircle::MythosCircle(vec2f pos, float radius) {

	mCenter = pos;
	mRadius = radius;
}

int MythosCircle::inBounds(vec2f& pos) {

	return ((pos - mCenter).length() <= mRadius);
}



MythosRhombus::MythosRhombus(vec2f p0, vec2f p1, vec2f p2) {

	mCenter = p0;
	mDir1 = p1 - p0;
	mDir2 = p2 - p0;

	if (mDir1.x * mDir2.y == mDir1.y * mDir2.x)
		throw MythosError("Sides Cannot Be Parallel", "If you want to make a line, just make a line.");

	mInvDet = 1.0f / ((mDir1.x * mDir2.y) - (mDir1.y * mDir2.x));
}

int MythosRhombus::inBounds(vec2f& pos) {

	vec2f tPos = pos - mCenter;

	float a = mInvDet * tPos.dot(mDir1);
	float b = mInvDet * tPos.dot(mDir2);

	return (a >= 0.0f && a <= 1.0f && b >= 0.0f && b <= 1.0f);
}



MythosTriangle::MythosTriangle(vec2f p0, vec2f p1, vec2f p2) : MythosRhombus(p0, p1, p2) {}

int MythosTriangle::inBounds(vec2f& pos) {

	vec2f tPos = pos - mCenter;

	float a = mInvDet * tPos.dot(mDir1);
	float b = mInvDet * tPos.dot(mDir2);

	return (a >= 0.0f && b >= 0.0f && (a + b) <= 1.0f);
}