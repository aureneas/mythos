#include <cmath>
#include <unordered_map>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/graphics/texture.h"
#include "../../../include/mythos/point.h"
// TODO remove later
#include <iostream>

float y_scale;  // scale along the y-axis of screen
Point* angle;   // angle of view, represented as Point
static float theta; // angle of view (as last checked), represented in radians
ALLEGRO_TRANSFORM identity; // identity transform
ALLEGRO_TRANSFORM x_shear;  // shear + scale for object along x-axis
ALLEGRO_TRANSFORM y_shear;  // shear + scale for object along y-axis
ALLEGRO_TRANSFORM horiz;

void set_y_scale(float y) {
    y_scale = y;
}

void set_angle(Point* p) {
    angle = p;
}


Graphics::Graphics() {
    x_res = 1.0f;
    y_res = 1.0f;
    lighting = al_create_bitmap(engine::get_screen_width(), engine::get_screen_height());
    tint = al_map_rgba(0,0,0,0);
}

void Graphics::begin_frame() {
    al_set_target_bitmap(lighting);
    al_clear_to_color(tint);
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));

    al_identity_transform(&identity);
    al_scale_transform(&identity, x_res, y_res);
    al_identity_transform(&x_shear);
    al_identity_transform(&y_shear);
    al_identity_transform(&horiz);

    if (angle) {
        theta = atan2f(angle->y, angle->x);
        float sin_f = sinf(theta);
        float cos_f = cosf(theta);
        //std::cout << "angle = (" << angle->x << ", " << angle->y << ")\n\ttheta = " << theta << "\n";
        al_vertical_shear_transform(&x_shear, -atan2f(y_scale * cos_f, 1));
        //std::cout << "\ttheta_x = " << -atan2f(y_scale * cosf(theta), sinf(theta)) << "\n";
        al_scale_transform(&x_shear, sin_f, 1);
        al_vertical_shear_transform(&y_shear, -atan2f(y_scale * sin_f, 1));
        //std::cout << "\ttheta_y = " << atan2f(y_scale * sinf(theta), cosf(theta)) << "\n";
        al_scale_transform(&y_shear, -cos_f, 1);
        al_build_transform(&horiz, 0, 0, 1, y_scale, theta + 3.14159f);
    }

    al_use_transform(&identity);
}

void Graphics::end_frame() {
    draw_bitmap(lighting, 0, 0, 0);
}

void Graphics::draw_lighting(ALLEGRO_BITMAP* bmp, int x, int y, int flags) {
    al_set_target_bitmap(lighting);
    draw_bitmap(bmp, x, y, flags);
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
}


/**
 *  Draws the bitmap object to the screen.
 */
void Graphics::draw_bitmap(ALLEGRO_BITMAP* bmp, int x, int y, int flags) {
    al_draw_bitmap(bmp, x, y, flags);
}

/**
 *  Draws the bitmap object to the screen, tinted by the color.
 */
void Graphics::draw_tinted_bitmap(ALLEGRO_BITMAP* bmp, ALLEGRO_COLOR tint, int x, int y, int flags) {
    al_draw_tinted_bitmap(bmp, tint, x, y, flags);
}

/**
 *  Draws the bitmap object to the screen, scaled.
 */
void Graphics::draw_scaled_bitmap(ALLEGRO_BITMAP* bmp, int x, int y, int w, int h, int sw, int sh, int flags) {
    al_draw_scaled_bitmap(bmp, 0, 0, w, h, x, y, sw, sh, flags);
}

/**
 *  Draws the bitmap object to the screen, rotated and scaled.
 */
void Graphics::draw_scaled_rotated_bitmap(ALLEGRO_BITMAP* bmp, float cx, float cy, float dx, float dy,
                                float xscale, float yscale, float angle, int flags) {
    al_draw_scaled_rotated_bitmap(bmp, cx, cy, dx, dy, xscale, yscale, angle, flags);
}

/**
 *  Draws a filled rectangle with the chosen color.
 */
void Graphics::draw_filled_rectangle(int x, int y, int w, int h, ALLEGRO_COLOR tint) {
    al_draw_filled_rectangle(x, y, w, h, tint);
}

/**
 *  Draws the USTR string object to the screen.
 */
void Graphics::draw_ustr(ALLEGRO_FONT* font, ALLEGRO_COLOR tint, int x, int y, int flags, ALLEGRO_USTR* ustr) {
    al_draw_ustr(font, tint, x, y, flags, ustr);
}

/**
 *  Draws the USTR string object to the screen on multiple lines.
 */
void Graphics::draw_multiline_ustr(ALLEGRO_FONT* font, ALLEGRO_COLOR tint, int x, int y, int w, int h, int flags, ALLEGRO_USTR* ustr) {
    al_draw_multiline_ustr(font, tint, x, y, w, h, flags, ustr);
}



bool CompositeTexture::in_bounds(int x, int y) {
    for (std::vector<TextureDisplacement>::iterator it = texture.begin(); it != texture.end(); ++it)
        it->texture->in_bounds(x - it->disp.x, y - it->disp.y);
}

void CompositeTexture::draw(Graphics* g, int x, int y) {
    // TODO preserve transformation
    for (std::vector<TextureDisplacement>::iterator it = texture.begin(); it != texture.end(); ++it)
        it->texture->draw(g, x + it->disp.x, y + it->disp.y);
}


BitmapTexture::BitmapTexture(ALLEGRO_BITMAP* b) {
    bmp = b;
}

bool BitmapTexture::in_bounds(int x, int y) {
    return (x >= 0 && y >= 0 && x <= al_get_bitmap_width(bmp) && y <= al_get_bitmap_height(bmp));
}

void BitmapTexture::draw(Graphics* g, int x, int y) {
    g->draw_bitmap(bmp, x, y, 0);
}


TransformTexture::TransformTexture(ALLEGRO_BITMAP* b) : BitmapTexture(b) {}

bool TransformTexture::in_bounds(int x, int y) {
    float xf = x, yf = y;

    ALLEGRO_TRANSFORM trans;
    compose_transform(&trans);
    al_invert_transform(&trans);
    al_transform_coordinates(&trans, &xf, &yf);

    return BitmapTexture::in_bounds((int)xf, (int)yf);
}

void TransformTexture::draw(Graphics* g, int x, int y) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    compose_transform(&trans);
    al_translate_transform(&trans, x, y);

    ALLEGRO_TRANSFORM prev;
    al_copy_transform(&prev, al_get_current_transform());
    al_compose_transform(&trans, &prev);

    al_use_transform(&trans);
    g->draw_bitmap(bmp, 0, 0, 0);
    al_use_transform(&prev);
}


VerticalTexture::VerticalTexture(ALLEGRO_BITMAP* b, bool a) : TransformTexture(b) {
    axis = a;
}

void VerticalTexture::compose_transform(ALLEGRO_TRANSFORM* trans) {
    al_compose_transform(trans, (axis ? &x_shear : &y_shear));
}


HorizontalTexture::HorizontalTexture(ALLEGRO_BITMAP* b) : TransformTexture(b) {}

void HorizontalTexture::compose_transform(ALLEGRO_TRANSFORM* trans) {
    al_compose_transform(trans, &horiz);
}


AngledTexture::AngledTexture(ALLEGRO_BITMAP* b, bool a, int h) : VerticalTexture(b, a) {
    height = h;
}

void AngledTexture::compose_transform(ALLEGRO_TRANSFORM* trans) {
    ALLEGRO_TRANSFORM angle_t;
    al_copy_transform(&angle_t, &horiz);
    angle_t.m[axis ? 1 : 0][1] -= (float)height / al_get_bitmap_height(bmp);
    al_compose_transform(trans, &angle_t);
}


LightTexture::LightTexture(ALLEGRO_COLOR t, int s) {
    strength = s;
    tint = t;
}
