#include <GL/glew.h>
#include <GL/glfw3.h>

#include "mythos_core.h"

namespace mythos_core {

	GLFWwindow*		glWindow;
	MythosOptions*	opt;

	void init(MythosOptions* popt) {

		shutdown();
		opt = popt;

		// Initialize GL libraries

		if (!glfwInit())
			throw "GLFW failed to initialize.";

		glWindow = glfwCreateWindow(opt->windowWidth, opt->windowHeight, opt->windowTitle, nullptr, nullptr);
		if (!glWindow) {
			shutdown();
			throw "GLFW window failed to initialize.";
		}

		glfwMakeContextCurrent(glWindow);
		glewExperimental = true;

		if (glewInit() != GLEW_OK) {
			shutdown();
			throw "GLEW failed to initialize.";
		}

		// Initialize display stuff

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, opt->windowWidth, opt->windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
		glOrtho(0.0, opt->windowWidth, 0.0, opt->windowHeight, 100.1, -0.1);
		glTranslated(0.0, opt->windowHeight, 0.0);
		glScaled(opt->windowXRes, -opt->windowYRes, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	void shutdown() {

		// Free up stuff

		if (glWindow)
			glfwDestroyWindow(glWindow);

		glfwTerminate();

	}

}


int main() { return 0; }