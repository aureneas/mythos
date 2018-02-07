#include <cmath>
#include <queue>
#include <functional>

#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/form/engine.h"

namespace engine {


bool VolumeWidget::in_bounds(int x, int y) {
	Texture* dt = *texture;
	if (dt)
		return dt->in_bounds(x, y);
	return false;
}

void VolumeWidget::draw(Graphics* g, int x, int y) {
	Texture* dt = *texture;
	if (dt) dt->draw(g, x + crd.x, y + crd.y);
}


void ContVolumeLayer::update_frame() {
	// update

	Layer::update_frame();
}

void ContVolumeLayer::populate(Window* w, )

void ContVolumeLayer::shift_vertical(int dy) {
	int abs_dy = abs(dy);
	// remove <<dy>> horizontal windows at bottom/top of screen
	if (dy > 0) {
		for (int j = abs_dy - 1; j >= 0; --j)
			child.pop_back();
	} else {
		for (int j = abs_dy - 1; j >= 0; --j)
			child.pop_front();
	}

	// shift all windows vertically by <<dy>>
	for (WidgetList::iterator it = child.begin(); it != child.end(); ++it)
		(*it)->crd.y += dy;

	// insert <<dy>> horizontal windows at top/bottom of screen, and populate them with objects
	for (int j = abs_dy - 1; j >= 0; --j) {
		Window* nw = new Window(Point{ 0, j }); // TODO set with correct initial coordinates
		// TODO populate
	}
}

void set_center(const Point& p) {
	center = p;
	child.clear();
}

void set_zoom(float z) {
	zoom = z;
	child.clear();
}

}
