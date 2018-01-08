#ifndef MYTHOS_WIDGET_H
#define MYTHOS_WIDGET_H

#include <forward_list>
#include <list>
#include <memory>
#include <unordered_map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "texture.h"


// TODO adjust to avoid collisions in hashmap
// TODO event for loss of focus?
#define MYTHOS_EVENT_NULL					ALLEGRO_GET_EVENT_TYPE('N', 'U', 'L', 'L')
#define MYTHOS_EVENT_MOUSE_ENTER_WIDGET		ALLEGRO_GET_EVENT_TYPE('E', 'n', 't', 'W')
#define MYTHOS_EVENT_MOUSE_LEAVE_WIDGET		ALLEGRO_GET_EVENT_TYPE('L', 'e', 'a', 'W')
#define MYTHOS_EVENT_UNFOCUS_WIDGET			ALLEGRO_GET_EVENT_TYPE('U', 'n', 'f', 'W')


namespace engine {


struct Widget;

typedef int (*WidgetFunction)(Widget*, ALLEGRO_EVENT*);

typedef std::unordered_map<ALLEGRO_EVENT_TYPE, WidgetFunction> WidgetEventMap;


struct Widget {
    Point crd;
    int flags;

    WidgetEventMap events;

    Widget(Point, int = 0);
    void set_event(ALLEGRO_EVENT_TYPE, WidgetFunction);

    virtual int update_event(ALLEGRO_EVENT*, int, int);
    virtual bool in_bounds(int, int) { return false; }
    virtual void draw(Graphics*, int, int) = 0;
};


struct TextureWidget: public Widget {
    Texture* texture;

    TextureWidget(Texture*, Point, int = 0);
    bool in_bounds(int, int);
    virtual void draw(Graphics*, int, int);
};


typedef std::list<std::unique_ptr<Widget> > WidgetList;

struct Window: public Widget {
    WidgetList child;

    Window(Point);
    int update_event(ALLEGRO_EVENT*, int, int);
    bool in_bounds(int, int);
    virtual void draw(Graphics*, int, int);

    void insert(Widget*);
};


Widget* get_mouse_on();
void set_mouse_on(Widget*, ALLEGRO_EVENT*);

Widget* get_mouse_focus();
void set_mouse_focus(Widget*, ALLEGRO_EVENT*);


}


#endif // MYTHOS_WIDGET_H
