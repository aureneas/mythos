#ifndef MYTHOS_FORM_H
#define MYTHOS_FORM_H

#include <memory>
#include <allegro5/allegro.h>

#include "../graphics/texture.h"
#include "volume.h"


struct Container;

struct Form {
    Container* parent;
    Texture* texture;
    Volume vol;
};

struct LightSource: public Form {
    int strength;           // strength of light
    ALLEGRO_COLOR tint;     // tint of light
};

typedef std::vector<std::unique_ptr<Form> > ContainerFormList;

struct Container: virtual public Form {
    ContainerFormList child;
    Point angle;               // angle of view, represented as Point

    void sort();

    /**
     *  Transfers form from this into cont.
     *  @param cont     Container* to transfer form into
     *  @param form     Form* to be transferred between this and cont
     */
    void transfer(Container*, Form*);
};


#endif // MYTHOS_FORM_H
