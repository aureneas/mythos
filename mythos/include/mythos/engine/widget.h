#ifndef MYTHOS_WIDGET_H
#define MYTHOS_WIDGET_H

#include <forward_list>
#include <list>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "input.h"
#include "../graphics/graphics.h"

namespace engine {


struct Widget;

typedef void (*WidgetFunction)(Widget*);

struct Widget {
    Point crd;
    int flags;

    bool mouse_on;
    WidgetFunction func_mouse_on;
    WidgetFunction func_mouse_off;
    WidgetFunction func_mouse_down;
    WidgetFunction func_mouse_up;

    Widget(Point, int = 0, WidgetFunction = nullptr, WidgetFunction = nullptr, WidgetFunction = nullptr, WidgetFunction = nullptr);
    virtual int update_event(ALLEGRO_EVENT*, Input*, int, int);
    virtual bool in_bounds(int, int);
    virtual void draw(int, int) = 0;
};

struct BitmapWidget: public Widget {
    ALLEGRO_BITMAP* bmp;

    BitmapWidget(ALLEGRO_BITMAP*, Point, int = 0, WidgetFunction = nullptr, WidgetFunction = nullptr, WidgetFunction = nullptr, WidgetFunction = nullptr);
    bool in_bounds(int, int);
    virtual void draw(int, int);
};

struct ButtonWidget: public BitmapWidget {
    ALLEGRO_BITMAP* pressed_bmp;
    bool pressed;

    ButtonWidget(ALLEGRO_BITMAP*, ALLEGRO_BITMAP*, Point, int = 0, WidgetFunction = nullptr, WidgetFunction = nullptr, WidgetFunction = nullptr, WidgetFunction = nullptr);
    void draw(int, int);
};

struct TextWidget: public Widget {
    ALLEGRO_USTR* ustr;
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR tint;

    TextWidget(ALLEGRO_USTR*, ALLEGRO_FONT*, ALLEGRO_COLOR, Point, int = 0);
    virtual bool in_bounds(int, int);
    virtual void draw(int, int);
};

struct MultilineTextWidget: public TextWidget {
    int max_line_width;
    int line_padding;

    MultilineTextWidget(ALLEGRO_USTR*, ALLEGRO_FONT*, ALLEGRO_COLOR, Point, int, int = 0, int = 0);
    bool in_bounds(int, int);
    void draw(int, int);
};

template <typename N>
struct BarValueWidget: public Widget {
    ALLEGRO_COLOR tint;

    N* cur;
    N cur_last;
    N* max;
    N max_last;

    int width;
    int height;
    float perc;

    BarValueWidget(N*, N*, ALLEGRO_COLOR, Point, int, int, int = 0);
    void draw(int, int);
};

struct BarValueWidget<int>;
struct BarValueWidget<unsigned>;


typedef std::list<std::unique_ptr<Widget> > WidgetList;

struct Window: public Widget {
    WidgetList child;

    Window(Point);
    int update_event(ALLEGRO_EVENT*, Input*, int, int);
    virtual void draw(int, int);

    void insert(Widget*);
};


}


#endif // MYTHOS_WIDGET_H
