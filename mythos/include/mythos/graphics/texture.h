#ifndef MYTHOS_TEXTURE_H
#define MYTHOS_TEXTURE_H

#include <allegro5/allegro.h>
#include "graphics.h"
#include "../point.h"

void set_y_scale(float);
void set_angle(Point*);


struct Texture {
    virtual bool in_bounds(int, int) { return false; }
    virtual void draw(Graphics*, int, int) = 0;
};

struct BitmapTexture: public Texture {
    ALLEGRO_BITMAP* bmp;
    BitmapTexture(ALLEGRO_BITMAP*);
    virtual bool in_bounds(int, int);
    virtual void draw(Graphics*, int, int);
};

struct VerticalTexture: public BitmapTexture {
    bool axis;      // true if x-axis, false if y-axis
    VerticalTexture(ALLEGRO_BITMAP*, bool);
    bool in_bounds(int, int);
    void draw(Graphics*, int, int);
};

struct HorizontalTexture: public BitmapTexture {
    HorizontalTexture(ALLEGRO_BITMAP*);
    void draw(Graphics*, int, int);
};

#endif // MYTHOS_TEXTURE_H
