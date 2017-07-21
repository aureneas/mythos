#include "../../../include/mythos/engine/state.h"


namespace engine {


Layer::Layer() : Window(Point{ 0, 0 }) {
    // TODO stuff???
}

void Layer::update_frame() {
    for (AnimationList::iterator it = anim_list.begin(); it != anim_list.end(); ++it) {
        if ((*it)->update_frame())
            anim_list.erase(it--);
    }

    graphics.begin_frame();
    draw(&graphics, 0, 0);
    graphics.end_frame();
}

int Layer::update_event(ALLEGRO_EVENT* e, int x, int y) {
    for (WidgetList::reverse_iterator it = child.rbegin(); it != child.rend(); ++it) {
        if ((*it)->update_event(e, 0, 0))
            return 1;
    }
    return 0;
}

void Layer::insert_animation(Animation* anim) {
    anim_list.emplace_back(anim);
}


void State::update_frame() {
    Input::update_frame();
    for (LayerList::iterator it = layer_list.begin(); it != layer_list.end(); ++it) {
        (*it)->update_frame();
    }
}

void State::update_event(ALLEGRO_EVENT* e) {
    Input::update_event(e);
    for (LayerList::reverse_iterator it = layer_list.rbegin(); it != layer_list.rend(); ++it) {
        if ((*it)->update_event(e))
            break;
    }
}

void State::insert_layer(Layer* layer) {
    layer_list.emplace_front(layer);
}

void State::remove_layer(Layer* layer) {
    for (LayerList::iterator it = layer_list.begin(); it != layer_list.end(); ++it) {
        if (it->get() == layer) {
            layer_list.erase(it);
            return;
        }
    }
}


}
