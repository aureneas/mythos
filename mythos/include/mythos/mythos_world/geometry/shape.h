#pragma once

#include "..\_important_stuff.h"


#define __SAME_SIGN(a, b) (((a) > 0.0f && (b) > 0.0f) || ((a) < 0.0f && (b) < 0.0f))
#define __DIFF_SIGN(a, b) (((a) > 0.0f && (b) < 0.0f) || ((a) < 0.0f && (b) > 0.0f))


class MYTHOS_WORLD_API MythosShape {

	protected:

		vec2f			mCenter;

	public:

		vec2f&			getCenter(void) { return mCenter; }

		virtual int		inBounds(vec2f&) { return MYTHOS_FALSE; }
};

class MYTHOS_WORLD_API MythosCircle : public MythosShape {

	private:

		float			mRadius;

	public:

		MythosCircle(vec2f, float);

		int				inBounds(vec2f&);
};

class MYTHOS_WORLD_API MythosRhombus : public MythosShape {

	protected:

		vec2f			mDir1;
		vec2f			mDir2;

		float			mInvDet;

	public:

		MythosRhombus(vec2f, vec2f, vec2f);

		virtual int		inBounds(vec2f&);
};

class MYTHOS_WORLD_API MythosTriangle : public MythosRhombus {

	public:

		MythosTriangle(vec2f, vec2f, vec2f);

		int				inBounds(vec2f&);
};