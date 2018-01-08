#ifndef MYTHOS_FORM_H
#define MYTHOS_FORM_H

#include <memory>
#include <allegro5/allegro.h>

#include "../engine/texture.h"
#include "volume.h"

#define MYTHOS_EVENT_INTERACT_FORM		ALLEGRO_GET_EVENT_TYPE('I', 'n', 't', 'F')
// mouse click or interact button press
#define MYTHOS_EVENT_COLLISION_FORM		ALLEGRO_GET_EVENT_TYPE('C', 'o', 'l', 'F')
// another form attempts to enter boundary

struct Form;
struct Container;

typedef int (*FormFunction)(Form*, ALLEGRO_EVENT*);

typedef std::unordered_map<ALLEGRO_EVENT_TYPE, FormFunction> FormEventMap;

struct Form {
    Container*		parent;
    Texture*		texture;
    Volume*			vol;
    FormEventMap	events;
};


typedef std::vector<std::unique_ptr<Form> > ContainerFormList;

struct Container {
    ContainerFormList child;
    Vec2 angle;					// angle of view, represented as Vec2

    void sort();
    virtual void insert(Form*);
    virtual void remove(Form*);
    void set_position(Form*, Vec3);

    /**
     *  Transfers form from this into cont.
     *  @param cont     Container* to transfer form into
     *  @param form     Form* to be transferred between this and cont
     */
    void transfer(Container*, Form*);
};


#endif // MYTHOS_FORM_H
