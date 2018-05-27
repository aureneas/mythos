#pragma once

#include <unordered_set>
#include "mythos_important_stuff.h"
#include "utility/matrix.h"

enum MYTHOS_EVENT_KEY : int {
	MYTHOS_TIMER = 100,
	MYTHOS_KEY_PRESS = GLFW_PRESS,
	MYTHOS_KEY_REPEAT = GLFW_REPEAT,
	MYTHOS_KEY_RELEASE = GLFW_RELEASE,
	MYTHOS_MOUSE_PRESS = GLFW_PRESS + 3,
	MYTHOS_MOUSE_RELEASE = GLFW_RELEASE + 3,
	MYTHOS_MOUSE_MOVE = 5,
	MYTHOS_MOUSE_ENTER = 6,
	MYTHOS_MOUSE_EXIT = 7
};

enum MYTHOS_EVENT_RETURN : int {
	MYTHOS_ERROR,
	MYTHOS_CONTINUE,
	MYTHOS_STOP
};


typedef std::unordered_set<int> HeldButtonsSet;

class MYTHOS_CORE_API MythosButtonEvent {

	private:

		HeldButtonsSet	mButtons;

	public:

		void setButtonHeld(int, bool);

		bool getButtonHeld(int);

};

class MYTHOS_CORE_API MythosKeyEvent : public MythosButtonEvent {
	
	public:

		int key;
		int scancode;
		int mods;
};

class MYTHOS_CORE_API MythosMouseEvent : public MythosButtonEvent, public vec2i {
	
	public:

		int button;
		int mods;
};

struct MYTHOS_CORE_API MythosEvent {

	const MythosKeyEvent& key;
	const MythosMouseEvent& mouse;

	MythosEvent(MythosKeyEvent&, MythosMouseEvent&);
};


void mythosKeyCallback(GLFWwindow*, int, int, int, int);

void mythosMouseMoveCallback(GLFWwindow*, double, double);

void mythosMouseButtonCallback(GLFWwindow*, int, int, int);

void mythosMouseEnterCallback(GLFWwindow*, int);

// TODO scroll, joysticks