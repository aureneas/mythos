#ifndef MYTHOS_AREALAYER_H
#define MYTHOS_AREALAYER_H

#include "state.h"
#include "../form/area.h"

namespace engine {


struct FormWidget: public Widget {
    Form* form;

    void draw(Graphics*, int, int);
};

struct AreaLayer: public Layer {
    int screen_buffer;

    ALLEGRO_BITMAP* bmp_ground;
    ALLEGRO_BITMAP* bmp_shadow;
    ALLEGRO_BITMAP* bmp_silhouette;
    ALLEGRO_TRANSFORM trans;

    AreaWrapper wrap;

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
