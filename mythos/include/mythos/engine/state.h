#ifndef MYTHOS_STATE_H
#define MYTHOS_STATE_H

#include "../animation.h"
#include "widget.h"

namespace engine {


struct Layer: public Window {
    Graphics graphics;
    AnimationList anim_list;

    Layer();
    void update_frame();
    int update_event(ALLEGRO_EVENT*, Input*, int = 0, int = 0);

    void insert_animation(Animation*);
};

typedef std::list<std::unique_ptr<Layer> >          LayerList;

class State {
    protected:
        Input input;
        LayerList layer_list;
    public:
        void update_frame();
        void update_event(ALLEGRO_EVENT*);

        void remove_layer(Layer*);
};


}


#endif // MYTHOS_STATE_H
