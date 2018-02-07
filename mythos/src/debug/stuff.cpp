#include "../../include/debug/debug.h"

namespace engine {

Texture* m_off;
Texture* m_on;
Texture* m_down;


int mouse_enter_function(Widget* w, ALLEGRO_EVENT* e) {
	if (TextureWidget* t = dynamic_cast<TextureWidget*>(w)) {
		t->texture = m_on;
	}
	set_mouse_on(w, e);
	return 1;
}
int mouse_leave_function(Widget* w, ALLEGRO_EVENT* e) {
	if (TextureWidget* t = dynamic_cast<TextureWidget*>(w)) {
		t->texture = m_off;
	}
	return 1;
}
int mouse_down_function(Widget* w, ALLEGRO_EVENT* e) {
	if (TextureWidget* t = dynamic_cast<TextureWidget*>(w)) {
		t->texture = m_down;
	}
	return 1;
}
int mouse_up_function(Widget* w, ALLEGRO_EVENT* e) {
	if (TextureWidget* t = dynamic_cast<TextureWidget*>(w)) {
		t->texture = m_on;
	}
	return 1;
}


DebugLayer::DebugLayer() {
	Widget* w = new TextureWidget(m_off, Point(100, 100));
	w->set_event(MYTHOS_EVENT_MOUSE_ENTER_WIDGET, mouse_enter_function);
	w->set_event(MYTHOS_EVENT_MOUSE_LEAVE_WIDGET, mouse_leave_function);
	w->set_event(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, mouse_down_function);
	w->set_event(ALLEGRO_EVENT_MOUSE_BUTTON_UP, mouse_up_function);
	insert(w);
}

DebugState::DebugState() {
	ALLEGRO_BITMAP* bmp = al_load_bitmap("lib/debug/mouse_off.png");
	m_off = new BitmapTexture(bmp);
	bmp = al_load_bitmap("lib/debug/mouse_on.png");
	m_on = new BitmapTexture(bmp);
	bmp = al_load_bitmap("lib/debug/mouse_down.png");
	m_down = new BitmapTexture(bmp);

	insert_layer(new DebugLayer());
}


}
