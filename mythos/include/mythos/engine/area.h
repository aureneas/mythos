#ifndef MYTHOS_AREALAYER_H
#define MYTHOS_AREALAYER_H

#include "state.h"
#include "../form/area.h"

namespace engine {


struct TileWidget: public Widget {
    Tile* tile;

    TileWidget(Tile*, Point);
    void draw(Graphics*, int, int);
};

struct FormWidget: public Widget {
    Form* form;

    FormWidget(Form*, Point);
    void draw(Graphics*, int, int);
};


enum AREA_LAYER_INDICES : int {
    TOP = 0,
    BOTTOM = 1,
    LEFT = 2,
    RIGHT = 3
};

struct AreaLayer: public Layer {
    int screen_buffer;
    WidgetList::iterator tile_end;

    AreaWrapper wrap;

    Point center;
    float top_view;

    int indices[4];

    AreaLayer();
    void update_frame();
    ALLEGRO_TRANSFORM get_crd_transform();
    void push(int, int, int, int);
    void set_center(Point);
    void adjust_center(Point);
};


}


#endif // MYTHOS_AREALAYER_H
