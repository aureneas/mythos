#ifndef MYTHOS_ENGINE_H
#define MYTHOS_ENGINE_H

#include <allegro5/allegro.h>

#include "state.h"

namespace engine {


State* get_state();
void set_state(State*);


int get_screen_width();
int get_screen_height();


void init_allegro();
void init_timer();
void init_display();
void init_event_queue();
void init(char const*);

void run();

void destroy_allegro();


}


#endif // MYTHOS_ENGINE_H
