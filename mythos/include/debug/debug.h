#ifndef MDEBUG_DEBUG_H
#define MDEBUG_DEBUG_H

#include "../mythos/engine/state.h"

namespace engine {


struct DebugLayer: public Layer {
	DebugLayer();
};

class DebugState: public State {
	public:
		DebugState();
};


}

#endif // MDEBUG_DEBUG_H
