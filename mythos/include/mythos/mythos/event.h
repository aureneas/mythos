#pragma once

#include <unordered_set>
#include "_important_stuff.h"
#include "utility/matrix.h"


#define MYTHOS_TIMER			100
#define MYTHOS_KEY_PRESS		1
#define MYTHOS_KEY_REPEAT		2
#define MYTHOS_KEY_RELEASE		0
#define MYTHOS_MOUSE_PRESS		4
#define MYTHOS_MOUSE_RELEASE	3
#define MYTHOS_MOUSE_MOVE		5
#define MYTHOS_MOUSE_ENTER		6
#define MYTHOS_MOUSE_EXIT		7

typedef int MYTHOS_EVENT_KEY;


enum MYTHOS_EVENT_RETURN : int {
	MYTHOS_ERROR,
	MYTHOS_CONTINUE,
	MYTHOS_STOP
};


class MythosWidget;


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

class MYTHOS_CORE_API MythosMouseEvent : public MythosButtonEvent, public vec2f {
	
	public:

		int				button;
		int				mods;

		MythosWidget*	drag;
		vec2f			dragDisplacement;
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