#ifndef MYTHOS_FORM_H
#define MYTHOS_FORM_H

#include <memory>
#include <allegro5/allegro.h>

#include "volume.h"


struct Form {
    Volume vol;
};

struct LightSource: public Form {
    int strength;           // strength of light
    ALLEGRO_COLOR tint;     // tint of light
};

typedef std::vector<std::unique_ptr<Form> > ContainerFormList;

struct Container: virtual public Form {
    ContainerFormList forms;
    Point angle;               // angle of view, represented as Point

    void sort();
    void transfer(Container*, Form*);
};


#endif // MYTHOS_FORM_H
