#ifndef MYTHOS_ENGINE_H
#define MYTHOS_ENGINE_H

#include <allegro5/allegro.h>

#include "state.h"

namespace engine {


int get_screen_width();
int get_screen_height();

State* get_state();

void init();
void run();


}


#endif // MYTHOS_ENGINE_H
