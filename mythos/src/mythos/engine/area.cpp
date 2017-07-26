#include <cmath>
#include <queue>
#include <functional>

#include "../../../include/mythos/engine/area.h"
#include "../../../include/mythos/engine/engine.h"

// TODO delete later
#include <iostream>

namespace engine {

struct comp_form_z {
    operator()(Form*& f1, Form*& f2) {
        return (f1->vol.z < f2->vol.z);
    }
};


ALLEGRO_COLOR white;
ALLEGRO_COLOR white_transparent;
ALLEGRO_COLOR black;


FormWidget::FormWidget(Form* f, Point p) : Widget(p) {
    form = f;
}

void FormWidget::draw(Graphics* g, int x, int y) {
    if (form->texture)  form->texture->draw(g, x + crd.x, y + crd.y);
}


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

    screen_buffer = 50;
}

void AreaLayer::update_frame() {
    graphics.begin_frame();
    Layer::update_frame();
/*
    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();

    // do the lighting
    for (LightSourceList::iterator it = light.begin(); it != light.end(); ++it) {
        float r, g, b, a;
        al_unmap_rgba_f((*it)->tint, &r, &b, &g, &a);

        Point lp = (*it)->vol.base.center;
        int lz = (*it)->vol.z;

        // draw ground lighting
        al_set_target_bitmap(bmp_ground);
        al_clear_to_color(black);// TODO white
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
            // actually draw the tiles here

        // draw shadows
        al_set_target_bitmap(bmp_shadow);
        al_clear_to_color(white_transparent);
        al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_ONE, ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
            // (actually draw the shadows here)
        for (WidgetList::iterator it = child.begin(); it != child.end(); ++it) {
            if (FormWidget* fw = dynamic_cast<FormWidget*>(it->get())) {
                float t = (float)lz / (lz - fw->form->vol.z);
                float xb = lp.x - (t * (lp.x - fw->form->vol.base.center.x));
                float yb = lp.y - (t * (lp.y - fw->form->vol.base.center.y));
                al_transform_coordinates(&crd_to_screen, &xb, &yb);
                t = (float)lz / (lz - fw->form->vol.height);
                float xt = lp.x - (t * (lp.x - fw->form->vol.base.center.x));
                float yt = lp.y - (t * (lp.y - fw->form->vol.base.center.y));
                al_transform_coordinates(&crd_to_screen, &xt, &yt);
                ALLEGRO_TRANSFORM shadow_shear;
                shadow_shear.m[1][1] = (yb - yt) / fw->form->vol.height;
                al_horizontal_shear_transform(&shadow_shear, atan2(yb - yt, xb - xt));
                //al_translate_transform(&shadow_shear, xt, yt);
                al_use_transform(&shadow_shear);
                fw->form->texture->draw(&graphics, (int)xt, (int)yt);
            }
        }

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
        for (LightSourceList::iterator it = light.begin(); it != light.end(); ++it) {
            // (actually draw the silhouettes here)
        }
    //}

    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_INVERSE_SRC_COLOR);
    graphics.draw_bitmap(graphics.lighting, 0, 0, 0);
    graphics.draw_bitmap(bmp_silhouette, 0, 0, 0);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    */
}


ALLEGRO_TRANSFORM AreaLayer::get_crd_transform() {
    ALLEGRO_TRANSFORM crd_to_screen;
    al_identity_transform(&crd_to_screen);
    al_translate_transform(&crd_to_screen, -center.x, -center.y);
    al_rotate_transform(&crd_to_screen, atan2(wrap.area->angle.y, wrap.area->angle.x));
    al_scale_transform(&crd_to_screen, -1, -top_view);
    al_translate_transform(&crd_to_screen, get_screen_width() * 0.5, get_screen_height() * 0.5);
    return crd_to_screen;
}

void AreaLayer::set_center(Point p) {
    set_angle(&wrap.area->angle);
    set_y_scale(top_view);

    center = p;
    child.clear();
    Point angle = wrap.area->angle;
    Point nrangle = { angle.y, -angle.x }; // angle, but rotated counter-clockwise by 90 degrees

    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();
    ALLEGRO_TRANSFORM screen_to_crd;
    al_copy_transform(&screen_to_crd, &crd_to_screen);
    al_scale_transform(&screen_to_crd, graphics.x_res, graphics.y_res);
    al_invert_transform(&screen_to_crd);

    float x_crd = -screen_buffer, y_crd = -screen_buffer;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner_t = { (int)x_crd, (int)y_crd };
    int top_side = -(angle * corner_t);
    int horiz_side1 = nrangle * corner_t;
    x_crd = (get_screen_width() / graphics.x_res) + screen_buffer;
    y_crd = (get_screen_height() / graphics.y_res) + screen_buffer;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner_b = { (int)x_crd, (int)y_crd };
    int bottom_side = -(angle * corner_b);
    int horiz_side2 = nrangle * corner_b;
    int left_side = std::min(horiz_side1, horiz_side2);
    int right_side = std::max(horiz_side1, horiz_side2);
    //std::cout << "Top Left: (" << corner_t.x << ", " << corner_t.y << ")\n";
    //std::cout << "Bottom Right: (" << corner_b.x << ", " << corner_b.y << ")\n";
    //std::cout << "TOP: " << top_side << "\nLEFT: " << left_side << "\nBOTTOM: " << bottom_side << "\nRIGHT: " << right_side << "\n";

    tile_layer = new Layer();
    AWTileList::iterator tt1_end = wrap.tile.lower_bound(bottom_side);
    for (AWTileList::iterator tt1 = wrap.tile.lower_bound(top_side); tt1 != tt1_end; ++tt1) {
        AWInnerTileList::iterator tt2_end = tt1->second.lower_bound(right_side);
        for (AWInnerTileList::iterator tt2 = tt1->second.lower_bound(left_side); tt2 != tt2_end; ++tt2) {
            //std::cout << "(" << tt1->first << ", " << tt2->first << ") -> " << tt2->second << "\n";
            float ar = (float)angle.y / angle.x;
            y_crd = -(tt1->first + (ar * tt2->first)) / (angle.x + (ar * angle.y));
            x_crd = -((float)tt1->first / angle.x) - (ar * y_crd);
            //std::cout << "(" << tt1->first << ", " << tt2->first << ") -> (" << x_crd << ", " << y_crd << ") -> ";
            al_transform_coordinates(&crd_to_screen, &x_crd, &y_crd);
            //std::cout << "(" << x_crd << ", " << y_crd << ")\n";
            tile_layer->insert(new TextureWidget(tt2->second->texture, Point{ (int)x_crd, tt2->second->elevation + (int)y_crd }));
        }
    }
    insert(tile_layer);

    AWFormList::iterator it1_end = wrap.form.lower_bound(bottom_side);
    for (AWFormList::iterator it1 = wrap.form.lower_bound(top_side); it1 != it1_end; ++it1) {
        std::priority_queue<Form*, std::vector<Form*>, comp_form_z> sort_by_z;
        AWInnerFormList::iterator it2_end = it1->second.lower_bound(right_side);
        for (AWInnerFormList::iterator it2 = it1->second.lower_bound(left_side); it2 != it2_end; ++it2)
            sort_by_z.push(it2->second);
        while (!sort_by_z.empty()) {
            Form* f = sort_by_z.top();
            x_crd = f->vol.base.center.x;
            y_crd = f->vol.base.center.y;
            //std::cout << "(" << x_crd << ", " << y_crd << ") -> ";
            al_transform_coordinates(&crd_to_screen, &x_crd, &y_crd);
            //std::cout << "(" << (int)x_crd << ", " << (int)y_crd << ")\n";
            insert(new FormWidget(f, Point{ (int)x_crd, (int)y_crd - f->vol.z - f->vol.height }));
            if (LightSource* l = static_cast<LightSource*>(f))
                light.push_back(l);
            sort_by_z.pop();
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
