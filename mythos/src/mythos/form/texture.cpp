#include "../../../include/mythos/form/texture.h"

#define TILE_WIDTH 32   // TODO remove later

#ifdef TILE_WIDTH


//ALLEGRO_TRANSFORM

float top_value;
float sin_value, cos_value;

void set_top_value(float t) {
    top_value = t;
}

void set_sin_value(float s) {
    sin_value = s;
}

void set_cos_value(float c) {
    cos_value = c;
}



/**
 *  Draws a shear texture.
 *  @param x        x coordinate to draw at
 *  @param y        y coordinate to draw at
 *  @param w        width of shear texture (in area)
 *  @param h        height of shear texture (in area)
 */
void ShearTexture::draw(int x, int y, int w, int h) {

}

#endif // TILE_WIDTH
