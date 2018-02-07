#include <cmath>
#include <unordered_map>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/engine/texture.h"
#include "../../../include/mythos/utility/matrix.h"
// TODO remove later
#include <iostream>


ALLEGRO_TRANSFORM identity;

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
    al_use_transform(&identity);
}

void Graphics::end_frame() {
    draw_bitmap(lighting, 0, 0, 0);
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
    for (std::vector<TextureDisplacement>::iterator it = texture.begin(); it != texture.end(); ++it) {
        if (it->texture->in_bounds(x - it->disp.x, y - it->disp.y))
			return true;
    }
    return false;
}

void CompositeTexture::draw(Graphics* g, int x, int y) {
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


TextTexture::TextTexture(ALLEGRO_USTR* t, ALLEGRO_FONT* f, ALLEGRO_COLOR c) {
	text = t;
	font = f;
	color = c;
	evaluate_text();
}

bool TextTexture::in_bounds(int x, int y) {
	return (x >= 0 && y <= 0 && x <= line_width && y <= line_height);
}

void TextTexture::draw(Graphics* g, int x, int y) {
	g->draw_ustr(font, color, x, y, 0, text);
}

void TextTexture::evaluate_text() {
	line_width = al_get_ustr_width(font, text);
	line_height = al_get_font_line_height(font);
}


MultilineTextTexture::MultilineTextTexture(ALLEGRO_USTR* t, ALLEGRO_FONT* f, ALLEGRO_COLOR c, int w, int h) : TextTexture(t, f, c) {
	line_width = w;
	line_height = h;
	evaluate_text();
}

bool MultilineTextTexture::in_bounds(int x, int y) {
	return (x >= 0 && y >= 0 && x <= line_width && y <= (line_height * line_num));
}

void MultilineTextTexture::draw(Graphics* g, int x, int y) {
	g->draw_multiline_ustr(font, color, x, y, line_width, line_height, 0, text);
}

bool count_ustr_lines(int num_lines, const ALLEGRO_USTR* line, void* extra) {
    int* ptr = (int*)extra;
    *ptr = num_lines;
    return true;
}

void MultilineTextTexture::evaluate_text() {
	al_do_multiline_ustr(font, line_width, text, count_ustr_lines, &line_num);
}


TransformTexture::TransformTexture(Texture* t) {
	texture = t;
}

bool TransformTexture::in_bounds(int x, int y) {
    float xf = x, yf = y;

    ALLEGRO_TRANSFORM trans;
    compose_transform(&trans);
    al_invert_transform(&trans);
    al_transform_coordinates(&trans, &xf, &yf);

    return texture->in_bounds(roundf(xf), roundf(yf));
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
    texture->draw(g, 0, 0);
    al_use_transform(&prev);
}
