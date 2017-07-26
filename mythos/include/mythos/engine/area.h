#ifndef MYTHOS_AREALAYER_H
#define MYTHOS_AREALAYER_H

#include "state.h"
#include "../form/area.h"

namespace engine {


struct FormWidget: public Widget {
    Form* form;

    FormWidget(Form*, Point);
    void draw(Graphics*, int, int);
};


typedef std::list<LightSource*> LightSourceList;

struct AreaLayer: public Layer {
    int screen_buffer;

    Layer* tile_layer;
    ALLEGRO_BITMAP* bmp_ground;
    ALLEGRO_BITMAP* bmp_shadow;
    ALLEGRO_BITMAP* bmp_silhouette;

    AreaWrapper wrap;
    LightSourceList light;

    Point center;
    float top_view;

    AreaLayer();
    void update_frame();

    ALLEGRO_TRANSFORM get_crd_transform();
    void set_center(Point);
    void adjust_center(Point);
};


}


#endif // MYTHOS_AREALAYER_H
