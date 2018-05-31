#pragma once

#include "..\_important_stuff.h"


class MythosForm;

class MYTHOS_WORLD_API MythosFormData {

public:
	virtual int inBounds(const vec3f&);
	virtual MYTHOS_EVENT_RETURN update(MYTHOS_EVENT_KEY, MythosForm*, const MythosEvent&);
	virtual void render(void);
};