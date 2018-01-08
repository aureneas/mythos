#include <cmath>
#include <queue>
#include <functional>

#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/form/engine.h"

// TODO delete later
#include <iostream>

namespace engine {


struct comp_form_z {
    operator()(Form*& f1, Form*& f2) {
        return (f1->vol->crd.z < f2->vol->crd.z);
    }
};


ALLEGRO_COLOR white;
ALLEGRO_COLOR white_transparent;
ALLEGRO_COLOR black;


TileWidget::TileWidget(Tile* t, Point p) : Widget(p) {
    tile = t;
}

void TileWidget::draw(Graphics* g, int x, int y) {
    tile->texture->draw(g, x + crd.x, y + crd.y);
}


FormWidget::FormWidget(Form* f, Point p) : Widget(p) {
    form = f;
}

bool FormWidget::in_bounds(int x, int y) {
	if (form->texture)
		return form->texture->in_bounds(x, y);
	return false;
}

void FormWidget::draw(Graphics* g, int x, int y) {
    if (form->texture) form->texture->draw(g, x + crd.x, y + crd.y);
}


AreaLayer::AreaLayer() {
    white = al_map_rgb(255, 255, 255);
    white_transparent = al_map_rgba(255, 255, 255, 0);
    black = al_map_rgb(0, 0, 0);

    graphics.tint = al_map_rgba(0,0,0,0);
    screen_buffer = 50;
}

void AreaLayer::update_frame() {
    adjust_center({ 1, 0 });
    Layer::update_frame();
}


ALLEGRO_TRANSFORM AreaLayer::get_crd_transform() {
    ALLEGRO_TRANSFORM crd_to_screen;
    al_identity_transform(&crd_to_screen);
    al_translate_transform(&crd_to_screen, -center.x, -center.y);
    al_rotate_transform(&crd_to_screen, atan2(wrap.area->angle.y, wrap.area->angle.x));
    al_scale_transform(&crd_to_screen, 1, -top_view);
    al_translate_transform(&crd_to_screen, get_screen_width() * 0.5, get_screen_height() * 0.5);
    return crd_to_screen;
}

void AreaLayer::push(int side_top, int side_bottom, int side_left, int side_right) {
    Point angle = wrap.area->angle;
    float vy = (pow(angle.x, 2) + pow(angle.y, 2));
    float vx = angle.x / vy;
    vy = angle.y / vy;
    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();
    float x_crd, y_crd;

    AWTileList::iterator tt1_end = wrap.tile.lower_bound(side_bottom);
    for (AWTileList::iterator tt1 = wrap.tile.lower_bound(side_top); tt1 != tt1_end; ++tt1) {
        AWInnerTileList::iterator tt2_end = tt1->second.lower_bound(side_right);
        for (AWInnerTileList::iterator tt2 = tt1->second.lower_bound(side_left); tt2 != tt2_end; ++tt2) {
            //std::cout << "(" << tt1->first << ", " << tt2->first << ") -> " << tt2->second << "\n";
            x_crd = -(tt1->first * vx) + (tt2->first * vy);
            y_crd = -(tt1->first * vy) - (tt2->first * vx);
            /*
            float ar = (float)angle.y / angle.x;
            y_crd = -(tt1->first + (ar * tt2->first)) / (angle.x + (ar * angle.y));
            x_crd = -((float)tt1->first / angle.x) - (ar * y_crd);*/
            //std::cout << "    (" << tt1->first << ", " << tt2->first << ") -> (" << x_crd << ", " << y_crd << ") -> ";
            al_transform_coordinates(&crd_to_screen, &x_crd, &y_crd);
            //std::cout << "(" << x_crd << ", " << y_crd << ")\n";
            child.emplace(tile_end, new TileWidget(tt2->second, Point{ (int)roundf(x_crd), (int)roundf(y_crd) - tt2->second->elevation }));
        }
    }

    WidgetList::iterator cur = tile_end;
    AWFormList::iterator it1_end = wrap.form.lower_bound(side_bottom);
    for (AWFormList::iterator it1 = wrap.form.lower_bound(side_top); it1 != it1_end; ++it1) {
        while (cur != child.end()) {
            if (FormWidget* fw = dynamic_cast<FormWidget*>(cur->get())) {
                if ((fw->form->vol->crd * angle) > it1->first + fw->form->vol->crd.z)
                    break;
            }
            ++cur;
        }

        std::priority_queue<Form*, std::vector<Form*>, comp_form_z> sort_by_z;
        AWInnerFormList::iterator it2_end = it1->second.lower_bound(side_right);
        for (AWInnerFormList::iterator it2 = it1->second.lower_bound(side_left); it2 != it2_end; ++it2)
            sort_by_z.push(it2->second);
        while (!sort_by_z.empty()) {
            Form* f = sort_by_z.top();
            x_crd = f->vol->crd.x;
            y_crd = f->vol->crd.y;
            al_transform_coordinates(&crd_to_screen, &x_crd, &y_crd);
            child.emplace(cur, new FormWidget(f, Point{ (int)roundf(x_crd), (int)roundf(y_crd) - f->vol->crd.z - f->vol->dim.z }));
            if (tile_end == child.end())
                --tile_end;
            //std::cout << "(" << x_crd << ", " << y_crd << ") -> ";
            //std::cout << "(" << (int)x_crd << ", " << (int)y_crd << ")\n";
            sort_by_z.pop();
        }
    }
}

void AreaLayer::set_center(Point p) {
    center = p;
    child.clear();
    tile_end = child.begin();
    Point angle = wrap.area->angle;
    Point nrangle = { angle.y, -angle.x }; // angle, but rotated counter-clockwise by 90 degrees
    screen_buffer = (int)wrap.area->width;

    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();
    ALLEGRO_TRANSFORM screen_to_crd;
    al_copy_transform(&screen_to_crd, &crd_to_screen);
    al_scale_transform(&screen_to_crd, graphics.x_res, graphics.y_res);
    al_invert_transform(&screen_to_crd);

    float x_crd = -screen_buffer, y_crd = -screen_buffer;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner_t = { (int)x_crd, (int)y_crd };
    indices[TOP] = -(angle * corner_t);
    int horiz_side1 = nrangle * corner_t;
    x_crd = (get_screen_width() / graphics.x_res) + screen_buffer;
    y_crd = (get_screen_height() / graphics.y_res) + screen_buffer;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner_b = { (int)x_crd, (int)y_crd };
    indices[BOTTOM] = -(angle * corner_b);
    int horiz_side2 = nrangle * corner_b;
    indices[LEFT] = std::min(horiz_side1, horiz_side2);
    indices[RIGHT] = std::max(horiz_side1, horiz_side2);
    //std::cout << "Top Left: (" << corner_t.x << ", " << corner_t.y << ")\n";
    //std::cout << "Bottom Right: (" << corner_b.x << ", " << corner_b.y << ")\n";
    //std::cout << "TOP: " << top_side << "\nLEFT: " << left_side << "\nBOTTOM: " << bottom_side << "\nRIGHT: " << right_side << "\n";

    push(indices[TOP], indices[BOTTOM], indices[LEFT], indices[RIGHT]);
}

void AreaLayer::adjust_center(Point p) {
    Point angle = wrap.area->angle;
    Point nrangle = { angle.y, -angle.x }; // angle, but rotated counter-clockwise by 90 degrees
    screen_buffer = (int)wrap.area->width;

    ALLEGRO_TRANSFORM crd_to_screen = get_crd_transform();

    // calculate crd change for existing widgets
    float x1 = 2*center.x, y1 = 2*center.y, x2 = (2*center.x) + p.x, y2 = (2*center.y) + p.y;
    //std::cout << "((" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ")) -> ";
    al_transform_coordinates(&crd_to_screen, &x1, &y1);
    al_transform_coordinates(&crd_to_screen, &x2, &y2);
    //std::cout << "((" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ")) -> ";
    Point dp = { (int)(roundf(x1) - roundf(x2)), (int)(roundf(y1) - roundf(y2)) };
    //std::cout << "<" << dp.x << ", " << dp.y << ">\n";
    center += p;

    // calculate top, bottom, left, and right
        // TODO make more efficient???
    ALLEGRO_TRANSFORM screen_to_crd;
    al_copy_transform(&screen_to_crd, &crd_to_screen);
    al_scale_transform(&screen_to_crd, graphics.x_res, graphics.y_res);
    al_invert_transform(&screen_to_crd);
    float x_crd = -screen_buffer - dp.x, y_crd = -screen_buffer - dp.y;
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner_t = { (int)x_crd, (int)y_crd };
    int top_side = -(angle * corner_t);
    int horiz_side1 = nrangle * corner_t;
    x_crd = (get_screen_width() / graphics.x_res) + screen_buffer - dp.x;
    y_crd = (get_screen_height() / graphics.y_res) + screen_buffer - dp.y;

    // adjust existing widgets, remove if off-screen
    for (WidgetList::iterator it = child.begin(); it != child.end(); ++it) {
        Point* c = &((*it)->crd);
        *c += dp;
        if (c->x < -screen_buffer || c->x > x_crd || c->y < -screen_buffer || c->y > y_crd) {
            //std::cout << "Erased at (" << c->x << ", " << c->y << ").\n";
            if (it == tile_end)
                ++tile_end;
            child.erase(it--);
        }
    }

    // finish calculating top, bottom, left, right
    al_transform_coordinates(&screen_to_crd, &x_crd, &y_crd);
    Point corner_b = { (int)x_crd, (int)y_crd };
    int bottom_side = -(angle * corner_b);
    int horiz_side2 = nrangle * corner_b;
    int left_side = std::min(horiz_side1, horiz_side2);
    int right_side = std::max(horiz_side1, horiz_side2);

    // insert any new textures that have come on-screen
    if (left_side < indices[LEFT])
        push(std::max(indices[TOP], top_side), std::min(indices[BOTTOM], bottom_side), left_side, std::min(indices[LEFT], right_side));
    else if (right_side > indices[RIGHT])
        push(std::max(indices[TOP], top_side), std::min(indices[BOTTOM], bottom_side), std::max(left_side, indices[RIGHT]), right_side);
    if (top_side < indices[TOP])
        push(top_side, std::min(indices[TOP], bottom_side), left_side, right_side);
    else if (bottom_side > indices[BOTTOM])
        push(std::max(indices[BOTTOM], top_side), bottom_side, left_side, right_side);

    indices[TOP] = top_side;
    indices[BOTTOM] = bottom_side;
    indices[LEFT] = left_side;
    indices[RIGHT] = right_side;
}



}
