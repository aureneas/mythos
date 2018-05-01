#ifndef MYTHOS_EVENT_H
#define MYTHOS_EVENT_H

#include "../../../_dependencies/GL/glfw3.h"

#include "../utility/matrix.h"

namespace mythos_engine {

	void event_init();


	enum MYTHOS_EVENT_TYPE : int {
		MYTHOS_EVENT_KEY_DOWN				= 0,
		MYTHOS_EVENT_KEY_UP					= 1,
		MYTHOS_EVENT_KEY_HOLD				= 2,
		MYTHOS_EVENT_MOUSE_BUTTON_DOWN		= 3,
		MYTHOS_EVENT_MOUSE_BUTTON_UP		= 4,
		MYTHOS_EVENT_MOUSE_MOTION			= 5
	};

	
	Vec2& get_mouse_coord();

	unsigned char get_last_key_pressed();


	void mythos_reshape(int, int);

	void mythos_keyboard_down(unsigned char, int, int);

	void mythos_keyboard_up(unsigned char, int, int);

	void mythos_keyboard(GLFWwindow*, int, int, int, int);

	void mythos_mouse(GLFWwindow*, int, int, int);

	void mythos_motion(GLFWwindow*, double, double);

}

#endif