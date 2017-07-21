#include "../../../include/mythos/engine/vnovel.h"
#include "../../../include/mythos/engine/engine.h"
#include "../../../include/mythos/graphics/graphics.h"

namespace engine {


ALLEGRO_BITMAP* SceneLayer::name_box;
ALLEGRO_FONT*   SceneLayer::name_font;

ALLEGRO_BITMAP* SceneLayer::text_box;
ALLEGRO_FONT*   SceneLayer::text_font;
ALLEGRO_COLOR   SceneLayer::text_color;

ALLEGRO_BITMAP* SceneLayer::choice_box;
ALLEGRO_BITMAP* SceneLayer::choice_mouseover;

int             SceneLayer::edge_padding;
int             SceneLayer::sprite_padding;



ChoiceWidget::ChoiceWidget(vnovel::Scene* s, vnovel::Choice* c, Point p, int f) :
        BitmapWidget(SceneLayer::choice_box, p, f, wfunc_choicewidget_mouse_on, wfunc_choicewidget_mouse_off, wfunc_choicewidget_mouse_down, wfunc_choose_scene_option),
        text(c->text, SceneLayer::text_font, SceneLayer::text_color, Point{ al_get_bitmap_width(bmp) / 2, al_get_bitmap_height(bmp) / 2 }, ALLEGRO_ALIGN_CENTER) {
    scene = s;
    opt = c;
}

void ChoiceWidget::draw(Graphics* g, int x, int y) {
    BitmapWidget::draw(g, x, y);
    text.draw(g, x + crd.x, y + crd.y);
}


static SceneLayer* scene_layer;

SceneLayer::SceneLayer(vnovel::Scene* s) : name_w({ 0, 0 }), text_w({ 0, 0 }) {
    set_scene(s);
    scene_layer = this;
}

SceneLayer::~SceneLayer() {
    delete scene;
    scene = nullptr;
    scene_layer = nullptr;
}

void SceneLayer::update_frame() {
    int xpos = edge_padding;
    for (std::list<vnovel::Character>::iterator it = scene->chara[vnovel::LEFT].begin(); it != scene->chara[vnovel::LEFT].end(); ++it) {
        graphics.draw_tinted_bitmap(it->bmp, it->tint, xpos, it->ypos, 0);
        xpos += sprite_padding;
    }
    xpos = get_screen_width() - al_get_bitmap_width(scene->chara[vnovel::RIGHT].front().bmp) - edge_padding;
    for (std::list<vnovel::Character>::iterator it = scene->chara[vnovel::RIGHT].begin(); it != scene->chara[vnovel::RIGHT].end(); ++it) {
        graphics.draw_tinted_bitmap(it->bmp, it->tint, xpos, it->ypos, 0);
        xpos -= sprite_padding;
    }

    xpos = (get_screen_width() / 2) - (((sprite_padding * (scene->chara[vnovel::CENTER].size() - 1)) + al_get_bitmap_width(scene->chara[vnovel::CENTER].front().bmp)) / 2);
    for (std::list<vnovel::Character>::iterator it = scene->chara[vnovel::CENTER].begin(); it != scene->chara[vnovel::CENTER].end(); ++it) {
        graphics.draw_tinted_bitmap(it->bmp, it->tint, xpos, it->ypos, 0);
        xpos += sprite_padding;
    }

    if (!scene->options.empty() && child.empty()) {
        for (std::list<vnovel::Choice>::iterator it = scene->options.begin(); it != scene->options.end(); ++it)
            insert(new ChoiceWidget(scene, &*it, Point{ 0, 0 }));
    }
    Layer::update_frame();
}

int SceneLayer::update_event(ALLEGRO_EVENT* e, int x, int y) {
    if (scene->options.empty()) {
        if (e->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP || e->type == ALLEGRO_EVENT_KEY_UP)
            advance();
    } else {
        Layer::update_event(e, x, y);
    }
    return 1;
}

void SceneLayer::draw(Graphics* g, int x, int y) {
    Layer::draw(g, x, y);
    name_w.draw(g, crd.x + x, crd.y + y);
    text_w.draw(g, crd.x + x, crd.y + y);
}

void SceneLayer::set_scene(vnovel::Scene* s) {
    child.clear();
    scene = s;

    // TODO can change colors of name and text
    // TODO change points
    name_w.child.clear();
    name_w.insert(new BitmapWidget(name_box, Point{ 0, 0 }));
    name_w.insert(new TextWidget(scene->name, name_font, al_map_rgb(255, 255, 255), Point{ 0, 0 }));
    text_w.child.clear();
    text_w.insert(new BitmapWidget(text_box, Point{ 0, 0 }));
    text_w.insert(new TextWidget(scene->text, text_font, al_map_rgb(255, 255, 255), Point{ 0, 0 }));

    advance();
}

void SceneLayer::advance() {
    Animation* anim = scene->advance();
    if (anim) {
        this->insert_animation(anim);
    } else {
        get_state()->remove_layer(this);
    }
}


void wfunc_choicewidget_mouse_on(Widget* w) {

}
void wfunc_choicewidget_mouse_off(Widget* w) {

}
void wfunc_choicewidget_mouse_down(Widget* w) {

}

void wfunc_choose_scene_option(Widget* w) {
    if (ChoiceWidget* c = dynamic_cast<ChoiceWidget*>(w)) {
        c->scene->change_scene(c->opt->after);
        scene_layer->set_scene(c->scene);
    }
}


}
