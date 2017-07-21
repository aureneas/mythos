#ifndef MYTHOS_GRAPHICS_H
#define MYTHOS_GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


struct Graphics {
    ALLEGRO_BITMAP* lighting;
    ALLEGRO_COLOR tint;

    Graphics();
    void begin_frame();
    void end_frame();

    void draw_lighting(ALLEGRO_BITMAP*, int, int, int);

    void draw_bitmap(ALLEGRO_BITMAP*, int, int, int);
    void draw_tinted_bitmap(ALLEGRO_BITMAP*, ALLEGRO_COLOR, int, int, int);
    void draw_scaled_bitmap(ALLEGRO_BITMAP*, int, int, int, int, int, int, int);
    void draw_scaled_rotated_bitmap(ALLEGRO_BITMAP*, float, float, float, float, float, float, float, int);

    void draw_filled_rectangle(int, int, int, int, ALLEGRO_COLOR);

    void draw_ustr(ALLEGRO_FONT*, ALLEGRO_COLOR, int, int, int, ALLEGRO_USTR*);
    void draw_multiline_ustr(ALLEGRO_FONT*, ALLEGRO_COLOR, int, int, int, int, int, ALLEGRO_USTR*);
};



#endif // MYTHOS_GRAPHICS_H
