#ifndef MYTHOS_MOUSE_HANDLERS_H
#define MYTHOS_MOUSE_HANDLERS_H

#include "../widget.h"

namespace mythos_engine {

	/*
	*	MOUSE_BUTTON_DOWN function, used if the Widget is draggable.
	*/
	int mouse_button_down_draggable(Widget*, Vec2);

	/*
	*	MOUSE_BUTTON_UP function, used if the Widget is draggable.
	*/
	int mouse_button_up_draggable(Widget*, Vec2);

	/*
	*	MOUSE_MOTION_PRESSED function, used if the Widget is draggable.
	*/
	int mouse_motion_pressed_draggable(Widget*, Vec2);

}

#endif MYTHOS_MOUSE_HANDLERS_H