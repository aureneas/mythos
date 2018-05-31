#pragma once

#include <memory>
#include "..\_important_stuff.h"

template <typename T>
class MythosContainer {

	public:
		virtual MythosContainer<T>*		addChild(T*) { return nullptr; }
		virtual std::shared_ptr<T>		removeChild(T*) { return std::shared_ptr<T>(nullptr); }
};