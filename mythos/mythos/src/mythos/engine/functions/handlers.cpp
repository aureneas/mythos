#include <GL/glut.h>

#include "../../../../include/mythos/engine/functions/mouse_handlers.h"

namespace mythos_engine {

	Widget* w_currently_dragged;
	Vec2 w_currently_dragged_rel_coord;

	bool mouse_in_bounds(Widget* w, Vec2 p) {
		return w->in_bounds(get_mouse_coord() - p);
	}

	int mouse_button_down_draggable(Widget* w, Vec2 p) {
		if (mouse_in_bounds(w, p)) {
			w_currently_dragged = w;
			w_currently_dragged_rel_coord = w->crd - get_mouse_coord();
			return MYTHOS_UPDATE_STOP;
		}
		return MYTHOS_UPDATE_CONTINUE;
	}

	int mouse_button_up_draggable(Widget* w, Vec2 p) {
		if (w == w_currently_dragged) {
			w_currently_dragged = nullptr;
			return MYTHOS_UPDATE_STOP;
		}
		return MYTHOS_UPDATE_CONTINUE;
	}

	int mouse_motion_pressed_draggable(Widget* w, Vec2 p) {
		if (w_currently_dragged) {
			w_currently_dragged->crd = get_mouse_coord() + w_currently_dragged_rel_coord;
			return MYTHOS_UPDATE_STOP;
		}
		return MYTHOS_UPDATE_CONTINUE;
	}

}