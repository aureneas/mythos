#ifndef MYTHOS_AREALAYER_H
#define MYTHOS_AREALAYER_H

#include "state.h"
#include "../form/area.h"

namespace engine {


struct TextureFormWidget: public Widget {

};

struct AreaLayer: public Layer {
    ALLEGRO_BITMAP* bmp_ground;
    ALLEGRO_BITMAP* bmp_shadow;
    ALLEGRO_BITMAP* bmp_silhouette;
    ALLEGRO_TRANSFORM trans;

    Area* area;

    Point center;
    float top_view;

    AreaLayer(Area*);
    void update_frame();

    void set_center(Point);
    void adjust_center(Point);
};


}


#endif // MYTHOS_AREALAYER_H
