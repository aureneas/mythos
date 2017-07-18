#include "../../../include/mythos/engine/input.h"

namespace engine {


void Input::update_frame() {
    for (std::unordered_map<int, unsigned>::iterator it = keyboard.begin(); it != keyboard.end(); ++it)
        ++(it->second);
}

int Input::update_event(ALLEGRO_EVENT* e) {
    switch (e->type) {
        case ALLEGRO_EVENT_MOUSE_AXES:
            mouse.x = e->mouse.x;
            mouse.y = e->mouse.y;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            while (buttons.size() <= e->mouse.button)
                buttons.push_back(false);
            buttons[e->mouse.button] = (e->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            keyboard.emplace(e->keyboard.keycode, 1);
            break;
        case ALLEGRO_EVENT_KEY_UP:
            keyboard.erase(e->keyboard.keycode);
            break;
    }
    return 0;
    // TODO later?
}

unsigned Input::get_key(int key) {
    std::unordered_map<int, unsigned>::iterator it = keyboard.find(key);
    return ((it == keyboard.end()) ? 0 : it->second);
}

bool Input::get_button(unsigned button) {
    return ((button >= buttons.size()) ? false : buttons[button]);
}


}
