#pragma once

#include "..\_important_stuff.h"

class MythosForm;

class MYTHOS_WORLD_API MythosFormListener {

	public:

		virtual void call(MythosForm*) = 0;
};

template <typename T>
struct MYTHOS_WORLD_API MythosParameterFormListener : public MythosFormListener {

private:

	typedef void (*MythosFormListenerFunc)(MythosForm*, T)

	T mParam;


};