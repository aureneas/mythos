#include <stdlib.h> // TODO i don't remember what i used this for
	// maybe it was for <chrono> ??
#include <thread>
#include <ctime>

#include "mythos_core.h"


MythosWindow* __MYTHOS_ROOT_WINDOW = nullptr;


MYTHOS_API void mythosInit() {

	// Initialize GL libraries

	if (!glfwInit())
		throw "GLFW failed to initialize.";
}


bool __MYTHOS_DO_NOT_TERMINATE_LOOP_FRAME;

void __mythosLoopFrame(void) {

	int frames_per_second = 60;

	std::chrono::duration<int, std::milli> frame_duration((int)std::round(1000.0 / frames_per_second));
	std::chrono::steady_clock::time_point nclock = std::chrono::steady_clock::now() + frame_duration;

	while (__MYTHOS_DO_NOT_TERMINATE_LOOP_FRAME) {
		std::chrono::steady_clock::time_point cclock = std::chrono::steady_clock::now();
		if (cclock > nclock)
			nclock = cclock + frame_duration - ((cclock - nclock) % frame_duration);
		std::this_thread::sleep_until(nclock);
		nclock = nclock + frame_duration;

		// update frame
		__MYTHOS_ROOT_WINDOW->update();

		// render frame
		__MYTHOS_ROOT_WINDOW->render();
	}
}

MYTHOS_API void mythosRun(MythosWindow* window) {

	__MYTHOS_ROOT_WINDOW = window;

	std::thread frameThread(__mythosLoopFrame);

	do {

		glfwWaitEvents();

	} while (!glfwWindowShouldClose(window->getWindow()));

	__MYTHOS_DO_NOT_TERMINATE_LOOP_FRAME = false;
	frameThread.join();

	mythosExit();
}


MYTHOS_API void mythosExit() {

	glfwTerminate();
}