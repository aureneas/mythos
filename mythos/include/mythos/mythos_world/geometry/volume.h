#pragma once

#include "shape.h"


class MYTHOS_WORLD_API MythosVolume {

	protected:

		vec3f					mCenter;

	public:

		MythosVolume(vec3f pos) { mCenter = pos; }

		virtual vec3f&			getCenter(void) { return mCenter; }

		virtual int				inBounds(vec3f&) { return MYTHOS_FALSE; }
};