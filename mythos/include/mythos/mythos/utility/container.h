#pragma once

#include "..\_important_stuff.h"

template <typename T>
class MythosContainer {

	public:
		virtual MythosContainer<T>*		addChild(T*) { return nullptr; }
		virtual void					removeChild(T*) = 0;
};