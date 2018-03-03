#include "../../../include/mythos/engine/widget.h"

//#include "../../../include/mythos/engine/state.h"
//#include "../../../include/mythos/debug/debug.h"

namespace mythos_engine {

	Widget::Widget(Vec2 p) {
		crd = p;
	}

	WidgetFunction Widget::get_event(MYTHOS_EVENT_TYPE type) {
		WidgetEvents::iterator it = events.find(type);
		if (it != events.end())
			return it->second;
		return nullptr;
	}

	void Widget::set_event(MYTHOS_EVENT_TYPE type, WidgetFunction value) {
		WidgetEvents::iterator it = events.find(type);
		if (it != events.end()) {
			events.erase(it);
			events.emplace_hint(it, type, value);
		}
		else {
			events.emplace(type, value);
		}
	}

	int Widget::update_event(MYTHOS_EVENT_TYPE type, Vec2 point) {
		WidgetFunction func = get_event(type);
		if (func) 
			return func(this, point);
		return MYTHOS_UPDATE_CONTINUE;
	}


	ContainerWidget::ContainerWidget(Vec2 p) : Widget(p) {}
	
	void ContainerWidget::update_frame() {
		// TODO implement?
	}

	int ContainerWidget::update_event(MYTHOS_EVENT_TYPE type, Vec2 point) {
		int value = Widget::update_event(type, point);
		if (value == MYTHOS_UPDATE_CONTINUE) {
			Vec2 n_point = point + crd;
			for (WidgetList::iterator it = child.begin(); it != child.end(); ++it) {
				value = (*it)->update_event(type, n_point);
				if (value != MYTHOS_UPDATE_CONTINUE)
					return value;
			}
		}
		return value;
	}

	bool ContainerWidget::in_bounds(Vec2 point) {
		Vec2 n_point = point - crd;
		for (WidgetList::iterator it = child.begin(); it != child.end(); ++it) {
			if ((*it)->in_bounds(n_point))
				return true;
		}
		return false;
	}

	void ContainerWidget::render() {
		glPushMatrix();
		glScaled(1.0, 1.0, 1.0 / length);
		glTranslated(crd.x, crd.y, 1.0);
		for (WidgetList::reverse_iterator it = child.rbegin(); it != child.rend(); ++it) {
			(*it)->render();
			glTranslated(0.0, 0.0, 1.0);
		}
		glPopMatrix();
	}

	void ContainerWidget::insert(Widget* w, int index) {
		if (index == 0)
			child.emplace_front(w);
		else if (index == length)
			child.emplace_back(w);
		else {
			WidgetList::iterator it = child.begin();
			for (int i = index - 1; i >= 0; --i)
				++it;
			child.emplace(it, w);
		}
		++length;
	}

	void ContainerWidget::remove(Widget* w) {
		for (WidgetList::iterator it = child.begin(); it != child.end(); ++it) {
			if (it->get() == w) {
				child.erase(it);
				--length;
				return;
			}
		}	
	}


	TextureWidget::TextureWidget(Texture* t, Vec2 p) : Widget(p) {
		texture = t;
	}
	
	bool TextureWidget::in_bounds(Vec2 point) {
		if (texture) 
			return texture->in_bounds(point - crd);
		return false;
	}

	void TextureWidget::render() {
		if (texture) {
			glPushMatrix();
			glTranslated(crd.x, crd.y, 0.0);
			texture->render();
			glPopMatrix();
		}
	}

}