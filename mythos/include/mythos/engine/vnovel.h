#ifndef MYTHOS_VNOVELLAYER_H
#define MYTHOS_VNOVELLAYER_H

#include "state.h"
#include "../vnovel/vnovel.h"

namespace engine {



struct ChoiceWidget: public BitmapWidget {
    vnovel::Scene* scene;
    vnovel::Choice* opt;
    TextWidget text;

    ChoiceWidget(vnovel::Scene*, vnovel::Choice*, Point, int = 0);
    void draw(Graphics*, int, int);
};

struct SceneLayer: public Layer {
    static ALLEGRO_BITMAP* name_box;
    static ALLEGRO_FONT* name_font;

    static ALLEGRO_BITMAP* text_box;
    static ALLEGRO_FONT* text_font;
    static ALLEGRO_COLOR text_color;

    static ALLEGRO_BITMAP* choice_box;
    static ALLEGRO_BITMAP* choice_mouseover;

    static int edge_padding;
    static int sprite_padding;

    Window name_w;
    Window text_w;

    vnovel::Scene* scene;

    SceneLayer(vnovel::Scene*);
    ~SceneLayer();
    void update_frame();
    int update_event(ALLEGRO_EVENT*, int = 0, int = 0);
    void draw(Graphics*, int, int);

    void set_scene(vnovel::Scene*);
    void advance();
};


void wfunc_choicewidget_mouse_on(Widget*);
void wfunc_choicewidget_mouse_off(Widget*);
void wfunc_choicewidget_mouse_down(Widget*);
void wfunc_choose_scene_option(Widget*);


}

#endif // MYTHOS_VNOVEL_H
