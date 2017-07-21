#include <cmath>

#include "../../../include/mythos/engine/area.h"
#include "../../../include/mythos/engine/engine.h"

namespace engine {

ALLEGRO_COLOR white;
ALLEGRO_COLOR white_transparent;
ALLEGRO_COLOR black;


AreaLayer::AreaLayer(Area* a) {
    white = al_map_rgb(255, 255, 255);
    white_transparent = al_map_rgba(255, 255, 255, 0);
    black = al_map_rgb(0, 0, 0);

    area = a;
    graphics.tint = al_map_rgba(0, 0, 0, 0);

    int width = get_screen_width();
    int height = get_screen_height();
    bmp_ground = al_create_bitmap(width, height);
    bmp_shadow = al_create_bitmap(width, height);
    bmp_silhouette = al_create_bitmap(width, height);
}

void AreaLayer::update_frame() {
    graphics.begin_frame();

    // draw tiles
    al_build_transform(&trans, 0.0f, 0.0f, 1.0f, top_view, atan2((float)-area->angle.y, (float)area->angle.x));

    Layer::update_frame();

    // do the lighting
    for (LightSourceList::iterator it = area->light_sources.begin(); it != area->light_sources.end(); ++it) {
        float r, g, b, a;
        al_unmap_rgba_f((*it)->tint, &r, &b, &g, &a);

        // draw ground lighting
        al_set_target_bitmap(bmp_ground);
        al_clear_to_color(white);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
            // actually draw the tiles here


        // draw shadows
        al_set_target_bitmap(bmp_shadow);
        al_clear_to_color(white_transparent);
        al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_ONE, ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
            // (actually draw the shadows here)

        al_set_target_bitmap(graphics.lighting);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_DST_COLOR, ALLEGRO_ZERO);
        graphics.draw_bitmap(bmp_ground, 0, 0, 0);
        graphics.draw_bitmap(bmp_shadow, 0, 0, 0);
    }

    // draw silhouettes
    al_set_target_bitmap(bmp_silhouette);
    al_clear_to_color(white_transparent);
    al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_CONST_COLOR, ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
    //for (TODO OBJECTS HERE) {
        for (LightSourceList::iterator it = area->light_sources.begin(); it != area->light_sources.end(); ++it) {
            // (actually draw the silhouettes here)
        }
    //}

    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_INVERSE_SRC_COLOR);
    graphics.draw_bitmap(graphics.lighting, 0, 0, 0);
    graphics.draw_bitmap(bmp_silhouette, 0, 0, 0);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
}

void AreaLayer::set_center(Point p) {
    center = p;
    child.clear();
}




}
