#include <stdlib.h> // TODO i don't remember what i used this for
	// maybe it was for <chrono> ??
#include <thread>
#include <ctime>

#include "mythos_core.h"

#include <iostream>
#include "shaders\mythos_shader.h"


MYTHOS_CORE_API void mythosInit() {

	// Initialize GL libraries

	glewExperimental = true; // we need this because we're using GLFW

	if (!glfwInit())
		throw MythosError("GLFW failed to initialize.");
}


bool __MYTHOS_DO_NOT_TERMINATE_LOOP_FRAME = true;

void __mythosLoopFrame(MythosWindow* window) {

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
		mythosTimerCallback(window);

		// render in the event loop
		glfwPostEmptyEvent();
	}
}

void __mythosLoopEvent(MythosWindow* window) {

	GLFWwindow* glWindow = window->getWindow();

	glfwMakeContextCurrent(glWindow);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(glWindow)) {

		glfwWaitEvents();

		// render frame
		window->render();
	}

	__MYTHOS_DO_NOT_TERMINATE_LOOP_FRAME = false;
}

MYTHOS_CORE_API void mythosRun(MythosWindow* window) {

	std::thread frameThread(__mythosLoopFrame, window);
	frameThread.detach();
	
	__mythosLoopEvent(window);

	mythosExit();
}


MYTHOS_CORE_API void mythosExit() {

	glfwTerminate();
}