#ifndef MYTHOS_AREA_H
#define MYTHOS_AREA_H

#include <map>
#include "form.h"


typedef std::list<LightSource*> LightSourceList;

struct Area: public Container {
    Texture*** tiles;
};



typedef std::multimap<int, Form*> AWInnerFormList;
typedef std::multimap<int, AWInnerFormList> AWFormList;

struct AreaWrapper {
    Area* area;
    Area* prox[8];

    AWFormList form;

    AWFormList::iterator find(int);
};


#endif // MYTHOS_AREA_H
