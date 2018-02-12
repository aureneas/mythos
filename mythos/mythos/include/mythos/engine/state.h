#ifndef MYTHOS_STATE_H
#define MYTHOS_STATE_H

#include "widget.h"

namespace mythos_engine {

	const ContainerWidget& get_layers();

	void push_layer(Widget*);

	void pop_layer();

}

#endif