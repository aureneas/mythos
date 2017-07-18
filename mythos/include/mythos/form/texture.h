#ifndef MYTHOS_TEXTURE_H
#define MYTHOS_TEXTURE_H

#include "form.h"

struct Texture {
    virtual void draw(int, int, int, int) = 0;
};

struct ShearTexture: public Texture {
    ALLEGRO_BITMAP* bmp;
    int width, height;

    void draw(int, int, int, int);
};


#endif // MYTHOS_TEXTURE_H
