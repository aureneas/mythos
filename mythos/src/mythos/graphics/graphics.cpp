#include <allegro5/allegro_primitives.h>
#include "../../../include/mythos/engine/engine.h"


void Graphics::begin_frame() {
    al_set_target_bitmap(lighting);
    al_clear_to_color(tint);
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
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
void draw_bitmap(ALLEGRO_BITMAP* bmp, int x, int y, int flags) {
    al_draw_bitmap(bmp, x, y, flags);
}

/**
 *  Draws the bitmap object to the screen, tinted by the color.
 */
void draw_tinted_bitmap(ALLEGRO_BITMAP* bmp, ALLEGRO_COLOR tint, int x, int y, int flags) {
    al_draw_tinted_bitmap(bmp, tint, x, y, flags);
}

/**
 *  Draws the bitmap object to the screen, rotated and scaled.
 */
void draw_scaled_rotated_bitmap(ALLEGRO_BITMAP* bmp, float cx, float cy, float dx, float dy,
                                float xscale, float yscale, float angle, int flags) {
    al_draw_scaled_rotated_bitmap(bmp, cx, cy, dx, dy, xscale, yscale, angle, flags);
}

/**
 *  Draws a filled rectangle with the chosen color.
 */
void draw_filled_rectangle(int x, int y, int w, int h, ALLEGRO_COLOR tint) {
    al_draw_filled_rectangle(x, y, w, h, tint);
}

/**
 *  Draws the USTR string object to the screen.
 */
void draw_ustr(ALLEGRO_FONT* font, ALLEGRO_COLOR tint, int x, int y, int flags, ALLEGRO_USTR* ustr) {
    al_draw_ustr(font, tint, x, y, flags, ustr);
}

/**
 *  Draws the USTR string object to the screen on multiple lines.
 */
void draw_multiline_ustr(ALLEGRO_FONT* font, ALLEGRO_COLOR tint, int x, int y, int w, int h, int flags, ALLEGRO_USTR* ustr) {
    al_draw_multiline_ustr(font, tint, x, y, w, h, flags, ustr);
}
