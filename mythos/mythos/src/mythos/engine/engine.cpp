#include <GL/glew.h>
#include <GL/glut.h>

#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/engine/event.h"
#include "../../../include/mythos/engine/options.h"
#include "../../../include/mythos/engine/state.h"
#include "../../../include/mythos/engine/texture.h"
#include "../../../include/mythos/engine/functions/mouse_handlers.h"

namespace mythos_engine {


	/*
	*	Initializes OpenGL.
	*/
	void mythos_init(int* argc, char** argv) {
		Options& opt = get_options();
		
		// initialize OpenGL and the window
		glutInit(argc, argv);
		load_options();
		glutInitWindowSize(opt.width, opt.height);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		
		/*   << BEGIN SKELETON CODE >>   */
		glutCreateWindow("<< WINDOW TITLE GOES HERE >>");
		/*    << END SKELETON CODE >>    */

		// set all event functions
		glutIdleFunc(mythos_idle);
		glutDisplayFunc(mythos_display);
		glutReshapeFunc(mythos_reshape);
		glutKeyboardFunc(mythos_keyboard_down);
		glutKeyboardUpFunc(mythos_keyboard_up);
		glutMouseFunc(mythos_mouse);
		glutMotionFunc(mythos_motion_pressed);
		glutPassiveMotionFunc(mythos_motion_unpressed);

		if (glewInit() != GLEW_OK)
			throw "GLEW didn't initialize properly.";
		init_texture();

		// TODO anti-aliasing?
		/*
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, opt.width, opt.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
		glOrtho(0.0, opt.width, 0.0, opt.height, 1.1, 0.0);
		glTranslated(0.0, opt.height, 0.0);
		glScaled(opt.resX, -opt.resY, -1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		event_init();
		get_layers().set_event(MYTHOS_EVENT_MOUSE_MOTION_PRESSED, mouse_motion_pressed_draggable);
	}

	void mythos_run(void) {
		glutMainLoop();
	}

}