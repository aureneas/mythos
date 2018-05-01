#include <stdlib.h>
#include <thread>
#include <ctime>
#include <cmath>

#include "../../../_dependencies/GL/glew.h"
#include "../../../_dependencies/GL/glfw3.h"

#include "../../include/engine/engine.h"
#include "../../include/engine/event.h"
#include "../../include/engine/options.h"
#include "../../include/engine/state.h"
#include "../../include/engine/texture.h"
#include "../../include/engine/functions/mouse_handlers.h"

namespace mythos_engine {


	const int frames_per_second = 30;

	GLFWwindow* glWindow = nullptr;


	/*
	*	Initializes OpenGL.
	*/
	void mythos_init(int* argc, char** argv) {
		Options& opt = get_options();
		

		// Initialize GL libraries

		if (!glfwInit())
			throw "GLFW failed to initialize.";

		glWindow = glfwCreateWindow(opt.width, opt.height, opt.title, nullptr, nullptr);
		if (!glWindow) {
			mythos_shutdown();
			throw "Window failed to initialize.";
		}
		glfwMakeContextCurrent(glWindow);
		glewExperimental = true;

		if (glewInit() != GLEW_OK)
			throw "GLEW failed to initialize.";


		// Initialize textures

		init_texture();


		// Initialize display stuff

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, opt.width, opt.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
		glOrtho(0.0, opt.width, 0.0, opt.height, 100.1, -0.1);
		glTranslated(0.0, opt.height, 0.0);
		glScaled(opt.resX, -opt.resY, -1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);


		// Initialize events

		event_init();
		//get_layers().set_event(MYTHOS_EVENT_MOUSE_MOTION_PRESSED, mouse_motion_pressed_draggable);
	}


	bool do_not_terminate_frame_loop = true;

	void mythos_frame_loop(void) {
		std::chrono::duration<int, std::chrono::milliseconds> frame_duration((int)std::round(1.0 / frames_per_second));
		std::chrono::steady_clock::time_point nclock = std::chrono::steady_clock::now() + frame_duration;

		ContainerWidget& layers = get_layers();

		while (do_not_terminate_frame_loop) {
			std::chrono::steady_clock::time_point cclock = std::chrono::steady_clock::now();
			if (cclock > nclock) 
				nclock = cclock + frame_duration - ((cclock - nclock) % frame_duration);
			std::this_thread::sleep_until(nclock);
			nclock = nclock + frame_duration;
			
			// update frame
			layers.update_frame();

			// render frame
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TODO figure out which i need
			layers.render();
			glfwSwapBuffers(glWindow);
		}
	}

	void mythos_event_loop(void) {
		
	}

	void mythos_run(void) {
		std::thread frame_thread(mythos_frame_loop);

		do {
			glfwWaitEvents();
		} while (!glfwWindowShouldClose(glWindow));

		do_not_terminate_frame_loop = false;
		frame_thread.join();

		mythos_shutdown();
	}


	void mythos_shutdown(void) {
		glfwTerminate();
	}

}