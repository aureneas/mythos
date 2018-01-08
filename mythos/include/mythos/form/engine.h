#ifndef MYTHOS_AREALAYER_H
#define MYTHOS_AREALAYER_H

#include "../engine/state.h"
#include "area.h"

namespace engine {


struct TileWidget: public Widget {
    Tile* tile;

    TileWidget(Tile*, Point);
    void draw(Graphics*, int, int);
};

struct FormWidget: public Widget {
    Form* form;

    FormWidget(Form*, Point);
    bool in_bounds(int, int);
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
    ALLEGRO_TRANSFORM get_crd_transform();	// constructs transformation from area coordinates to screen coordinates
    void push(int, int, int, int);			// inserts all objects in rectangle indicated by four values
    void set_center(Point);					// sets the center of view
    void adjust_center(Point);				// adjusts the center of view
};


}


#endif // MYTHOS_AREALAYER_H
