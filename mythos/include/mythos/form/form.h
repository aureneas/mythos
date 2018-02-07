#ifndef MYTHOS_FORM_H
#define MYTHOS_FORM_H

#include <memory>
#include <unordered_map>
#include <allegro5/allegro.h>

#include "../engine/texture.h"

#define MYTHOS_EVENT_INTERACT_FORM		ALLEGRO_GET_EVENT_TYPE('I', 'n', 't', 'F')
// mouse click or interact button press
#define MYTHOS_EVENT_COLLISION_FORM		ALLEGRO_GET_EVENT_TYPE('C', 'o', 'l', 'F')
// another form attempts to enter boundary

struct Form;
typedef int (*FormFunction)(Form*, void*);

typedef std::unordered_map<ALLEGRO_EVENT_TYPE, FormFunction> FormEventMap;

struct Form {
    Texture*		texture;
    FormEventMap	events;

    int execute(ALLEGRO_EVENT_TYPE, void*);
};


#endif // MYTHOS_FORM_H
