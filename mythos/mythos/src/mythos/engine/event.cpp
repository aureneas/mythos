#include <GL/glut.h>

#include "../../../include/mythos/engine/event.h"
#include "../../../include/mythos/engine/options.h"
#include "../../../include/mythos/engine/widget.h"

#include "../../../include/mythos/debug/debug_string.h"

namespace mythos_engine {

	Options options;

	/*
	*	Loads the default display options.
	*/
	void load_default_options() {
		/*   << BEGIN SKELETON CODE >>   */
		options.width = 640;
		options.height = 480;
		options.resX = 1.0;// options.width / 640.0;
		options.resY = 1.0;// options.height / 480.0;
		options.free_resize = false;
		/*    << END SKELETON CODE >>    */
	}

	/*
	*	Loads the options saved in "config.ini". Failing this, loads default options.
	*/
	void load_options() {
		// TODO implement
		load_default_options();
	}

	/*
	*	Getter function for current DisplayOptions
	*/
	Options& get_options() {
		return options;
	}


	ContainerWidget layers(Vec2(0, 0));

	const ContainerWidget& get_layers() {
		return layers;
	}

	void push_layer(Widget* w) {
		layers.child.emplace_front(w);
		++layers.length;
	}

	void pop_layer() {
		layers.child.pop_front();
		--layers.length;
	}


	int frame = 0;

	/*
	*	Handles idle time.
	*/
	void mythos_idle(void) {
		layers.update_frame();
		//++frame;
		glutPostRedisplay();
	}

	/*
	*	Handles display.
	*/
	void mythos_display(void) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		layers.render();
		glutSwapBuffers();
	}

	/*
	*	Handles reshaping of the window.
	*/
	void mythos_reshape(int n_width, int n_height) {
		if (options.free_resize) {
			options.width = n_width;
			options.height = n_height;
			options.resX = options.width / 320.0;
			options.resY = options.height / 240.0;

			glPopMatrix();
			glPushMatrix();
			glScaled(options.resX, options.resY, 1.0);

			glViewport(0, 0, n_width, n_height);
			glutPostRedisplay();
		}
		else {
			glViewport(0, 0, options.width, options.height);
			glutPostRedisplay();
		}
	}

	/*
	*	Handles keyboard down-presses.
	*/
	void mythos_keyboard_down(unsigned char key, int x, int y) {
		// TODO implement
		if (key == ' ')
			++frame;
		ShowText("Fucking shit", 50, 50);
		glutSwapBuffers();
		layers.update_event(MYTHOS_EVENT_KEY_DOWN, Vec2(0, 0), nullptr);
	}

	/*
	*	Handles keyboard up-presses.
	*/
	void mythos_keyboard_up(unsigned char key, int x, int y) {
		// TODO implement
		layers.update_event(MYTHOS_EVENT_KEY_UP, Vec2(0, 0), nullptr);
	}

	/*
	*	Handles mouse button presses.
	*	@param button		the button pressed (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON)
	*	@param state		whether the button was pressed (GLUT_DOWN) or released (GLUT_UP)
	*/
	void mythos_mouse(int button, int state, int x, int y) {
		// TODO implement
		layers.update_event(MYTHOS_EVENT_MOUSE_BUTTON_DOWN, Vec2(x, y), nullptr);
	}

	/*
	*	Handles mouse motion, with a button held down.
	*/
	void mythos_motion_pressed(int x, int y) {
		// TODO implement
		layers.update_event(MYTHOS_EVENT_MOUSE_MOTION, Vec2(x, y), nullptr);
	}

	/*
	*	Handles mouse motion, without a button held down.
	*/
	void mythos_motion_unpressed(int x, int y) {
		// TODO implement
		layers.update_event(MYTHOS_EVENT_MOUSE_MOTION, Vec2(x, y), nullptr);
	}

}