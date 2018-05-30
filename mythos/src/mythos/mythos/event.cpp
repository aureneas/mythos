#include <mythos\event.h>
#include <mythos\window.h>


MythosEvent::MythosEvent(MythosKeyEvent& k, MythosMouseEvent& m) : key(k), mouse(m) {}


void MythosButtonEvent::setButtonHeld(int button, bool isHeld) {

	if (isHeld) {

		mButtons.insert(button);
	}
	else {

		mButtons.erase(button);
	}
}

bool MythosButtonEvent::getButtonHeld(int button) {

	return (mButtons.find(button) != mButtons.end());
}


MythosKeyEvent		__MYTHOS_KEY_EVENT;
MythosMouseEvent	__MYTHOS_MOUSE_EVENT;
MythosEvent			__MYTHOS_EVENT(__MYTHOS_KEY_EVENT, __MYTHOS_MOUSE_EVENT);


void mythosTimerCallback(MythosWindow* window) {

	window->update(MYTHOS_TIMER, __MYTHOS_EVENT);
}

void mythosKeyCallback(GLFWwindow* glWindow, int key, int scancode, int action, int mods) {

	MythosWindow* window = mythosFindWindow(glWindow);

	if (window) {

		__MYTHOS_KEY_EVENT.key = key;
		__MYTHOS_KEY_EVENT.scancode = scancode;
		__MYTHOS_KEY_EVENT.mods = mods;

		unsigned c;
		// TODO figure out what the char is

		window->update((MYTHOS_EVENT_KEY)action, __MYTHOS_EVENT);
	}
}

void mythosMouseMoveCallback(GLFWwindow* glWindow, double x, double y) {
	
	MythosWindow* window = mythosFindWindow(glWindow);

	if (window) {

		__MYTHOS_MOUSE_EVENT.x = (int)std::round(x);
		__MYTHOS_MOUSE_EVENT.y = (int)std::round(y);

		window->update(MYTHOS_MOUSE_MOVE, __MYTHOS_EVENT);
	}
}

void mythosMouseButtonCallback(GLFWwindow* glWindow, int button, int action, int mods) {

	MythosWindow* window = mythosFindWindow(glWindow);

	if (window) {

		__MYTHOS_MOUSE_EVENT.button = button;
		__MYTHOS_MOUSE_EVENT.mods = mods;

		window->update((MYTHOS_EVENT_KEY)(action + 3), __MYTHOS_EVENT);
	}
}

void mythosMouseEnterCallback(GLFWwindow* glWindow, int entered) {

	MythosWindow* window = mythosFindWindow(glWindow);

	if (window) {

		window->update((entered ? MYTHOS_MOUSE_ENTER : MYTHOS_MOUSE_EXIT), __MYTHOS_EVENT);
	}
}