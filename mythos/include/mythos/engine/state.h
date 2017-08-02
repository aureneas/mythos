#ifndef MYTHOS_STATE_H
#define MYTHOS_STATE_H

#include "../animation.h"
#include "widget.h"

namespace engine {


struct Layer: public Window {
    Graphics graphics;
    AnimationList anim_list;

    Layer();
    virtual void update_frame();
    virtual int update_event(ALLEGRO_EVENT*, int = 0, int = 0);

    void insert_animation(Animation*);
};

typedef std::list<std::unique_ptr<Layer> >          LayerList;

class State {
    protected:
        LayerList layer_list;
    public:
        void update_frame();
        void update_event(ALLEGRO_EVENT*);

        void insert_layer(Layer*);
        void remove_layer(Layer*);
};


}


#endif // MYTHOS_STATE_H
