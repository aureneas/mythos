#include <allegro5/allegro_primitives.h>

#include "../../../include/mythos/engine/state.h"
#include "../../../include/mythos/engine/graphics.h"

namespace engine {


Widget* prev_mouse_on;
Widget* prev_mouse_focus;

Widget* get_mouse_on() {
	return prev_mouse_on;
}

void set_mouse_on(Widget* w, ALLEGRO_EVENT* e) {
	if (prev_mouse_on) {
		WidgetEventMap::iterator it = prev_mouse_on->events.find(MYTHOS_EVENT_MOUSE_LEAVE_WIDGET);
		if (it != prev_mouse_on->events.end())
			it->second(prev_mouse_on, e);
	}

	prev_mouse_on = w;
}

Widget* get_mouse_focus() {
	return prev_mouse_focus;
}

void set_mouse_focus(Widget* w, ALLEGRO_EVENT* e) {
	if (prev_mouse_focus) {
		WidgetEventMap::iterator it = prev_mouse_focus->events.find(MYTHOS_EVENT_UNFOCUS_WIDGET);
		if (it != prev_mouse_focus->events.end())
			it->second(prev_mouse_focus, e);
	}

	prev_mouse_focus = w;
}



Widget::Widget(Point p, int f) {
    crd = p;
    flags = f;
}

void Widget::set_event(ALLEGRO_EVENT_TYPE t, WidgetFunction f) {
	WidgetEventMap::iterator it = events.find(t);
	if (it != events.end())
		events.erase(it);

	events.emplace(t, f);
}

int Widget::update_event(ALLEGRO_EVENT* e, int x, int y) {
	x += crd.x;
	y += crd.y;

	ALLEGRO_EVENT_TYPE m = e->type;
	switch (e->type) {
		case ALLEGRO_EVENT_MOUSE_AXES:
			{
				bool mouse_on = in_bounds(e->mouse.x - x, e->mouse.y - y);
				if (prev_mouse_on == this && !mouse_on) {
					m = MYTHOS_EVENT_MOUSE_LEAVE_WIDGET;
					prev_mouse_on = nullptr;
				} else if (prev_mouse_on != this && mouse_on)
					m = MYTHOS_EVENT_MOUSE_ENTER_WIDGET;
				else if (!mouse_on)
					m = MYTHOS_EVENT_NULL;
				break;
			}
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			{
				if (!in_bounds(e->mouse.x - x, e->mouse.y - y))
					m = MYTHOS_EVENT_NULL;
				break;
			}
	}

	if (m != MYTHOS_EVENT_NULL) {
		WidgetEventMap::iterator it = events.find(m);
		if (it != events.end())
			return it->second(this, e);
	}
	return 0;
}

/*
int Widget::update_event(ALLEGRO_EVENT* e, int x, int y) {
    switch (e->type) {
        case ALLEGRO_EVENT_MOUSE_AXES:
            {
                if (func_mouse_on != nullptr || func_mouse_down != nullptr || func_mouse_up != nullptr || func_mouse_off != nullptr) {
                    if (!Input::get_button(0) && !Input::get_button(1)) {
                        if (mouse_on && !in_bounds(e->mouse.x - x, e->mouse.y - y)) {
                            if (func_mouse_off) func_mouse_off(this);
                            mouse_on = false;
                        } else if (!mouse_on && in_bounds(e->mouse.x - x, e->mouse.y - y)) {
                            if (func_mouse_on)  func_mouse_on(this);
                            mouse_on = true;
                        }
                    }
                }
                return 0;
            }
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (mouse_on) {
                    if (func_mouse_off)
                        func_mouse_off(this);
                    if (func_mouse_down)
                        func_mouse_down(this);
                }
                return 1;
            }
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            {
                if (in_bounds(e->mouse.x - x, e->mouse.y - y)) {
                    if (mouse_on && func_mouse_up != nullptr)
                        func_mouse_up(this);
                    if (func_mouse_on)
                        func_mouse_on(this);
                    mouse_on = true;
                }
                return 1;
            }
        default:
            return 0;
    }
}
*/


TextureWidget::TextureWidget(Texture* t, Point p, int f) : Widget(p, f) {
    texture = t;
}

bool TextureWidget::in_bounds(int x, int y) {
    if (texture)
		return texture->in_bounds(x, y);
    return false;
}

void TextureWidget::draw(Graphics* g, int x, int y) {
    if (texture)
		texture->draw(g, x + crd.x, y + crd.y);
}



Window::Window(Point p) : Widget(p) {
    // TODO other things here?
}

int Window::update_event(ALLEGRO_EVENT* e, int x, int y) {
    for (WidgetList::reverse_iterator it = child.rbegin(); it != child.rend(); ++it) {
		if ((*it)->update_event(e, x + crd.x, y + crd.y))
			return 1;
	}
    return Widget::update_event(e, x, y);
}

bool Window::in_bounds(int x, int y) {
	for (WidgetList::reverse_iterator it = child.rbegin(); it != child.rend(); ++it) {
		if ((*it)->in_bounds(x - (*it)->crd.x, y - (*it)->crd.y))
			return true;
	}
	return false;
}

void Window::draw(Graphics* g, int x, int y) {
    for (WidgetList::iterator it = child.begin(); it != child.end(); ++it)
        (*it)->draw(g, x + crd.x, y + crd.y);
}

void Window::insert(Widget* w) {
    child.emplace_back(w);
}



Layer::Layer() : Window(Point{ 0, 0 }) {
    // TODO stuff???
}

void Layer::update_frame() {
    for (AnimationList::iterator it = anim_list.begin(); it != anim_list.end(); ++it) {
        if ((*it)->update_frame())
            anim_list.erase(it--);
    }

    graphics.begin_frame();
    draw(&graphics, 0, 0);
    graphics.end_frame();
}

int Layer::update_event(ALLEGRO_EVENT* e, int x, int y) {
    for (WidgetList::reverse_iterator it = child.rbegin(); it != child.rend(); ++it) {
        if ((*it)->update_event(e, 0, 0)) {
			set_mouse_focus(it->get(), e);
			return 1;
        }
    }
    return Widget::update_event(e, 0, 0);
}

void Layer::insert_animation(Animation* anim) {
    anim_list.emplace_back(anim);
}



void State::update_frame() {
    for (LayerList::iterator it = layer_list.begin(); it != layer_list.end(); ++it) {
        (*it)->update_frame();
    }
}

void State::update_event(ALLEGRO_EVENT* e) {
    for (LayerList::reverse_iterator it = layer_list.rbegin(); it != layer_list.rend(); ++it) {
        if ((*it)->update_event(e))
            break;
    }
}

void State::insert_layer(Layer* layer) {
    layer_list.emplace_front(layer);
}

void State::remove_layer(Layer* layer) {
    for (LayerList::iterator it = layer_list.begin(); it != layer_list.end(); ++it) {
        if (it->get() == layer) {
            layer_list.erase(it);
            return;
        }
    }
}


}
