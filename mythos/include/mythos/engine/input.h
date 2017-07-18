#ifndef MYTHOS_INPUT_H
#define MYTHOS_INPUT_H

#include <vector>
#include <unordered_map>
#include <allegro5/allegro.h>

#include "../point.h"

namespace engine {


class Input {
    private:
        static Point mouse;
        static std::vector<bool> buttons;
        static std::unordered_map<int, unsigned> keyboard;
    public:
        void update_frame();
        int update_event(ALLEGRO_EVENT*);

        unsigned get_key(int);
        bool get_button(unsigned);
};


}

#endif // MYTHOS_INPUT_H
