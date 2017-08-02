#ifndef MYTHOS_TEXTURE_H
#define MYTHOS_TEXTURE_H

#include <vector>
#include <allegro5/allegro.h>
#include "graphics.h"
#include "../point.h"

void set_y_scale(float);
void set_angle(Point*);


struct Texture {
    virtual bool in_bounds(int, int) { return false; }
    virtual void draw(Graphics*, int, int) = 0;
};

struct CompositeTexture: public Texture {
    struct TextureDisplacement {
        Point disp;
        std::unique_ptr<Texture> texture;
    };

    std::vector<TextureDisplacement> texture;

    bool in_bounds(int, int);
    void draw(Graphics*, int, int);
};

struct BitmapTexture: public Texture {
    ALLEGRO_BITMAP* bmp;
    BitmapTexture(ALLEGRO_BITMAP*);
    virtual bool in_bounds(int, int);
    virtual void draw(Graphics*, int, int);
};

struct TransformTexture: public BitmapTexture {
    TransformTexture(ALLEGRO_BITMAP*);
    virtual void compose_transform(ALLEGRO_TRANSFORM*) = 0;
    bool in_bounds(int, int);
    void draw(Graphics*, int, int);
};

struct VerticalTexture: public TransformTexture {
    bool axis;      // true if x-axis, false if y-axis
    VerticalTexture(ALLEGRO_BITMAP*, bool);
    virtual void compose_transform(ALLEGRO_TRANSFORM*);
};

struct HorizontalTexture: public TransformTexture {
    HorizontalTexture(ALLEGRO_BITMAP*);
    void compose_transform(ALLEGRO_TRANSFORM*);
};

struct AngledTexture: public VerticalTexture {
    int height;
    AngledTexture(ALLEGRO_BITMAP*, bool, int);
    void compose_transform(ALLEGRO_TRANSFORM*);
};

struct LightTexture: public Texture {
    int strength;
    ALLEGRO_COLOR tint;
    LightTexture(ALLEGRO_COLOR, int);
    void draw(Graphics*, int, int) {}
};

#endif // MYTHOS_TEXTURE_H
