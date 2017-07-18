#ifndef MYTHOS_AREA_H
#define MYTHOS_AREA_H

#include "form.h"


typedef std::list<LightSource*> LightSourceList;

struct Area: public Container {
    LightSourceList light_sources;

};


#endif // MYTHOS_AREA_H
