#include <unordered_set>

#include "../../include/engine/event.h"
#include "../../include/engine/options.h"
#include "../../include/engine/widget.h"

//#include "../../../include/mythos/debug/debug.h"

namespace mythos_engine {

	GLdouble glModelViewMatrix[16];
	GLdouble glProjectionMatrix[16];
	GLint glViewportMatrix[4];


	Options options;

	/*
	*	Loads the default display options.
	*/
	void load_default_options() {
		/*   << BEGIN SKELETON CODE >>   */
		options.width = 800;
		options.height = 600;
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

	ContainerWidget& get_layers() {
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


	void event_init() {
		glGetDoublev(GL_MODELVIEW_MATRIX, glModelViewMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, glProjectionMatrix);
		glGetIntegerv(GL_VIEWPORT, glViewportMatrix);
	}


	Vec2 mouse_coord;

	void set_mouse_coord(double x, double y) {
		GLdouble x_temp, y_temp, z_temp;
		gluUnProject(x, y, 0.0, glModelViewMatrix, glProjectionMatrix, glViewportMatrix, &x_temp, &y_temp, &z_temp);
		mouse_coord.x = (int)round(x_temp);
		mouse_coord.y = options.height - (int)round(y_temp);
	}

	Vec2& get_mouse_coord() {
		return mouse_coord;
	}

	typedef std::unordered_set<unsigned char> KeysPressedSet;
	KeysPressedSet all_scancodes_pressed;

	unsigned char last_scancode_pressed;

	void set_key_status(unsigned char key, bool is_pressed) {
		if (is_pressed) {
			last_scancode_pressed = key;
			all_scancodes_pressed.insert(key);
		}
		else {
			all_scancodes_pressed.erase(key);
		}
	}

	unsigned char get_last_key_pressed() {
		return last_scancode_pressed;
	}


	/*
	*	Handles reshaping of the window.
	*/
	void mythos_reshape(int n_width, int n_height) {
		// TODO implement
		if (n_width != options.width || n_height != options.height)
			glutReshapeWindow(options.width, options.height);
	}

	/*
	*	Handles keyboard down-presses.
	*/
	void mythos_keyboard_down(unsigned char key, int x, int y) {
		set_key_status(key, true);
		layers.update_event(MYTHOS_EVENT_KEY_DOWN, Vec2(0, 0));
	}

	/*
	*	Handles keyboard up-presses.
	*/
	void mythos_keyboard_up(unsigned char key, int x, int y) {
		set_key_status(key, false);
		layers.update_event(MYTHOS_EVENT_KEY_UP, Vec2(0, 0));
	}

	/*
	*	Handles key presses, releases, and repeats.
	*/
	void mythos_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			layers.update_event(MYTHOS_EVENT_KEY_DOWN, Vec2(0, 0));
		}
		else if (action == GLFW_RELEASE) {
			layers.update_event(MYTHOS_EVENT_KEY_UP, Vec2(0, 0));
		}
		else {
			layers.update_event(MYTHOS_EVENT_KEY_HOLD, Vec2(0, 0));
		}
	}

	/*
	*	Handles mouse button presses.
	*	@param button		the button pressed (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON)
	*	@param state		whether the button was pressed (GLUT_DOWN) or released (GLUT_UP)
	*/
	void mythos_mouse(GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS)
			layers.update_event(MYTHOS_EVENT_MOUSE_BUTTON_DOWN, Vec2(0, 0));
		else
			layers.update_event(MYTHOS_EVENT_MOUSE_BUTTON_UP, Vec2(0, 0));
	}

	/*
	*	Handles mouse motion, with a button held down.
	*/
	void mythos_motion(GLFWwindow* window, double x, double y) {
		set_mouse_coord(x, y);
		layers.update_event(MYTHOS_EVENT_MOUSE_MOTION, Vec2(0, 0));
	}

}