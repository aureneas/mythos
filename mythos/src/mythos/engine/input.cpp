#include "../../../include/mythos/engine/input.h"

namespace engine {


Point                               Input::mouse;
std::vector<bool>                   Input::buttons;
std::unordered_map<int, unsigned>   Input::keyboard;


void Input::update_frame() {
    for (std::unordered_map<int, unsigned>::iterator it = keyboard.begin(); it != keyboard.end(); ++it)
        ++(it->second);
}

int Input::update_event(ALLEGRO_EVENT* e) {
    switch (e->type) {
        case ALLEGRO_EVENT_MOUSE_AXES:
            Input::mouse.x = e->mouse.x;
            Input::mouse.y = e->mouse.y;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            while (Input::buttons.size() <= e->mouse.button)
                Input::buttons.push_back(false);
            Input::buttons[e->mouse.button] = (e->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            Input::keyboard.emplace(e->keyboard.keycode, 1);
            break;
        case ALLEGRO_EVENT_KEY_UP:
            Input::keyboard.erase(e->keyboard.keycode);
            break;
    }
    return 0;
    // TODO later?
}

unsigned Input::get_key(int key) {
    std::unordered_map<int, unsigned>::iterator it = Input::keyboard.find(key);
    return ((it == Input::keyboard.end()) ? 0 : it->second);
}

bool Input::get_button(unsigned button) {
    return ((button >= Input::buttons.size()) ? false : Input::buttons[button]);
}


}
