#include <cmath>

#include "../../../include/mythos/engine/area.h"
#include "../../../include/mythos/engine/engine.h"

namespace engine {

ALLEGRO_COLOR white;
ALLEGRO_COLOR white_transparent;
ALLEGRO_COLOR black;


AreaLayer::AreaLayer() {
    white = al_map_rgb(255, 255, 255);
    white_transparent = al_map_rgba(255, 255, 255, 0);
    black = al_map_rgb(0, 0, 0);

    graphics.tint = al_map_rgba(0, 0, 0, 0);

    int width = get_screen_width();
    int height = get_screen_height();
    bmp_ground = al_create_bitmap(width, height);
    bmp_shadow = al_create_bitmap(width, height);
    bmp_silhouette = al_create_bitmap(width, height);
}

void AreaLayer::update_frame() {
    graphics.begin_frame();
/*
    // draw tiles
    al_build_transform(&trans, 0.0f, 0.0f, 1.0f, top_view, atan2((float)-wrap.area->angle.y, (float)wrap.area->angle.x));

    Layer::update_frame();

    // do the lighting
    for (LightSourceList::iterator it = wrap.area->light.begin(); it != wrap.area->light.end(); ++it) {
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
        for (LightSourceList::iterator it = wrap.area->light.begin(); it != wrap.area->light.end(); ++it) {
            // (actually draw the silhouettes here)
        }
    //}

    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_INVERSE_SRC_COLOR);
    graphics.draw_bitmap(graphics.lighting, 0, 0, 0);
    graphics.draw_bitmap(bmp_silhouette, 0, 0, 0);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);*/
}

ALLEGRO_TRANSFORM AreaLayer::get_crd_transform() {
    ALLEGRO_TRANSFORM crd_to_screen;
    al_translate_transform(&crd_to_screen, -center.x, -center.y);
    al_build_transform(&crd_to_screen, get_screen_width() * 0.5, get_screen_height() * 0.5, 1, top_view, atan2f(wrap.area->angle.y, wrap.area->angle.x));
    return crd_to_screen;
}

void AreaLayer::set_center(Point p) {
    center = p;
    child.clear();
    Point angle = wrap.area->angle;
    Point nrangle = { -angle.y, angle.x }; // angle, but rotated counter-clockwise by 90 degrees

    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();
    ALLEGRO_TRANSFORM screen_to_crd;
    al_copy_transform(&screen_to_crd, &crd_to_screen);
    al_scale_transform(&screen_to_crd, graphics.x_res, graphics.y_res);
    al_invert_transform(&screen_to_crd);

    float x_crd = -screen_buffer, y_crd = -screen_buffer;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner = { (int)x_crd, (int)y_crd };
    int top_side = angle * corner;
    int left_side = nrangle * corner;
    x_crd = (get_screen_width() / graphics.x_res) + screen_buffer;
    y_crd = (get_screen_height() / graphics.y_res) + screen_buffer;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    corner.x = (int)x_crd;
    corner.y = (int)y_crd;
    int bottom_side = angle * corner;
    int right_side = nrangle * corner;

    AWFormList::iterator it1_end = wrap.form.lower_bound(bottom_side);
    for (AWFormList::iterator it1 = wrap.form.lower_bound(top_side); it1 != it1_end; ++it1) {
        AWInnerFormList sort_by_z;
        AWInnerFormList::iterator it2_end = it1->second.lower_bound(right_side);
        for (AWInnerFormList::iterator it2 = it1->second.lower_bound(left_side); it2 != it2_end; ++it2)
            sort_by_z.emplace(it2->second->vol.z, it2->second);
        for (AWInnerFormList::iterator it2 = sort_by_z.begin(); it2 != sort_by_z.end(); ++it2) {
            Form* f = it2->second;
            x_crd = f->vol.base.center.x;
            y_crd = f->vol.base.center.y;
            al_transform_coordinates(&crd_to_screen, &x_crd, &y_crd);
            insert(new TextureWidget(f->texture, Point{ (int)x_crd, (int)y_crd - f->vol.z - f->vol.height }));
        }
    }
}

void AreaLayer::adjust_center(Point p) {
    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();
    float x1 = center.x, y1 = center.y, x2 = center.x + p.x, y2 = center.y + p.y;
    al_transform_coordinates(&crd_to_screen, &x1, &y1);
    al_transform_coordinates(&crd_to_screen, &x2, &y2);
    Point dp = { (int)x1 - (int)x2, (int)y1 - (int)y2 };
    for (WidgetList::iterator it = child.begin(); it != child.end(); ++it) {
        (*it)->crd += dp;
        // TODO remove if off-screen
    }
}




}
