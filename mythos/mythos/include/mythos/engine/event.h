#ifndef MYTHOS_EVENT_H
#define MYTHOS_EVENT_H

namespace mythos_engine {

	enum MYTHOS_EVENT : int {
		MYTHOS_EVENT_KEY_DOWN				= 0,
		MYTHOS_EVENT_KEY_UP					= 1,
		MYTHOS_EVENT_MOUSE_BUTTON_DOWN		= 2,
		MYTHOS_EVENT_MOUSE_BUTTON_UP		= 3,
		MYTHOS_EVENT_MOUSE_MOTION			= 4
	};


	void mythos_idle(void);

	void mythos_display(void);

	void mythos_reshape(int, int);

	void mythos_keyboard_down(unsigned char, int, int);

	void mythos_keyboard_up(unsigned char, int, int);

	void mythos_mouse(int, int, int, int);

	void mythos_motion_pressed(int, int);

	void mythos_motion_unpressed(int, int);

}

#endif