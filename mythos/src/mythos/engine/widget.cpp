#include <allegro5/allegro_primitives.h>

#include "../../../include/mythos/engine/widget.h"
#include "../../../include/mythos/graphics/graphics.h"

namespace engine {


Widget::Widget(Point p, int f, WidgetFunction m_on, WidgetFunction m_off, WidgetFunction m_down, WidgetFunction m_up) {
    crd = p;
    flags = f;

    func_mouse_on = m_on;
    func_mouse_off = m_off;
    func_mouse_down = m_down;
    func_mouse_up = m_up;
}

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


TextureWidget::TextureWidget(Texture* t, Point p, int f, WidgetFunction m_on, WidgetFunction m_off, WidgetFunction m_down, WidgetFunction m_up) : Widget(p, f, m_on, m_off, m_down, m_up) {
    texture = t;
}

bool TextureWidget::in_bounds(int x, int y) {
    return texture->in_bounds(x - crd.x, y - crd.y);
}

void TextureWidget::draw(Graphics* g, int x, int y) {
    texture->draw(g, x + crd.x, y + crd.y);
}


BitmapWidget::BitmapWidget(ALLEGRO_BITMAP* b, Point p, int f, WidgetFunction m_on, WidgetFunction m_off, WidgetFunction m_down, WidgetFunction m_up) : Widget(p, f, m_on, m_off, m_down, m_up) {
    bmp = b;
}

bool BitmapWidget::in_bounds(int x, int y) {
    return (x >= crd.x && x <= crd.x + al_get_bitmap_width(bmp) && y >= crd.y && y <= crd.y + al_get_bitmap_height(bmp));
}

void BitmapWidget::draw(Graphics* g, int x, int y) {
    if (bmp)    g->draw_bitmap(bmp, x + crd.x, y + crd.y, flags);
}


ButtonWidget::ButtonWidget(ALLEGRO_BITMAP* b1, ALLEGRO_BITMAP* b2, Point p, int f, WidgetFunction m_on, WidgetFunction m_off, WidgetFunction m_down, WidgetFunction m_up) : BitmapWidget(b1, p, f, m_on, m_off, m_down, m_up) {
    pressed_bmp = b2;
    pressed = false;
}

void ButtonWidget::draw(Graphics* g, int x, int y) {
    if (pressed) {
        if (pressed_bmp)    g->draw_bitmap(pressed_bmp, x + crd.x, y + crd.y, flags);
    } else
        BitmapWidget::draw(g, x, y);
}


TextWidget::TextWidget(ALLEGRO_USTR* u, ALLEGRO_FONT* f, ALLEGRO_COLOR t, Point p, int fl) : Widget(p, fl) {
    ustr = u;
    font = f;
    tint = t;
}

bool TextWidget::in_bounds(int x, int y) {
    if (y >= crd.y && y <= crd.y + al_get_font_line_height(font)) {
        int w = al_get_ustr_width(font, ustr);
        return ((flags & ALLEGRO_ALIGN_CENTER) ?
                (x >= crd.x - (w/2) && x <= crd.x + (w/2)) :
                (x >= crd.x && x <= crd.x + w));
    }
    return false;
}

void TextWidget::draw(Graphics* g, int x, int y) {
    if (ustr)   g->draw_ustr(font, tint, x + crd.x, y + crd.y, flags, ustr);
}


MultilineTextWidget::MultilineTextWidget(ALLEGRO_USTR* u, ALLEGRO_FONT* fnt, ALLEGRO_COLOR c, Point p, int l, int h, int f) :
        TextWidget(u, fnt, c, p, f) {
    max_line_width = l;
    line_padding = h;
}

bool count_ustr_lines(int line_num, const ALLEGRO_USTR* line, void* extra) {
    int* ptr = (int*)extra;
    *ptr = line_num;
    return true;
}

bool MultilineTextWidget::in_bounds(int x, int y) {
    if (x >= crd.x && x <= crd.x + max_line_width) {
        int num_lines;
        al_do_multiline_ustr(font, max_line_width, ustr, count_ustr_lines, &num_lines);
        return (y >= crd.y && y <= crd.y + (num_lines * line_padding));
    }
    return false;
}

void MultilineTextWidget::draw(Graphics* g, int x, int y) {
    if (ustr)   g->draw_multiline_ustr(font, tint, x + crd.x, y + crd.y, max_line_width, line_padding, flags, ustr);
}


template <typename N>
BarValueWidget<N>::BarValueWidget(N* c, N* m, ALLEGRO_COLOR t, Point p, int w, int h, int f) : Widget(p, f) {
    tint = t;
    cur = c;
    cur_last = *cur;
    max = m;
    max_last = *max;
    perc = (float)cur_last / max_last;
    width = w;
    height = h;
}

template <typename N>
void BarValueWidget<N>::draw(Graphics* g, int x, int y) {
    if (*cur != cur_last || *max != max_last) {
        cur_last = *cur;
        max_last = *max;
        perc = (float)cur_last / max_last;
    }
    // TODO make modifiable via flags
    int x_off = x + crd.x;
    int y_off = y + crd.y;
    g->draw_filled_rectangle(x_off, y_off, x_off + (width * perc), y_off + height, tint);
}


Window::Window(Point p) : Widget(p) {
    // TODO other things here?
}

int Window::update_event(ALLEGRO_EVENT* e, int x, int y) {
    if (in_bounds(e->mouse.x - x, e->mouse.y - y)) {
        for (WidgetList::reverse_iterator it = child.rbegin(); it != child.rend(); ++it) {
            if ((*it)->update_event(e, x, y))
                return 1;
        }
    }
    return 0;
}

void Window::draw(Graphics* g, int x, int y) {
    for (WidgetList::iterator it = child.begin(); it != child.end(); ++it)
        (*it)->draw(g, x + crd.x, y + crd.y);
}

void Window::insert(Widget* w) {
    child.emplace_back(w);
}


}
