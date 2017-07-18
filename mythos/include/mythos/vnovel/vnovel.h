#ifndef MYTHOS_VNOVEL_H
#define MYTHOS_VNOVEL_H

#include <vector>
#include <allegro5/allegro.h>
#include "../animation.h"

namespace vnovel {


enum Position {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2,
    OFFSCREEN = 3
};


struct Character {
    unsigned id;

    ALLEGRO_BITMAP* bmp;
    ALLEGRO_COLOR tint;
    int ypos;
};


struct Scene;

struct Instruction {
    virtual Animation* execute(Scene* s) { return nullptr; }
};

typedef std::vector<std::unique_ptr<Instruction> > InstructionList;

struct SceneData {
    std::list<Character> chara;
    InstructionList instruction;
};

struct Choice {
    SceneData* after;
    ALLEGRO_USTR* text;
};

struct Scene {
    std::list<Character> chara[4];
    std::list<Choice> options;

    ALLEGRO_USTR* name;
    ALLEGRO_USTR* text;

    InstructionList* instruction;
    InstructionList::iterator place;

    Animation* advance();
    Character* get(unsigned);
    void change_position(unsigned, Position);
    void change_scene(SceneData*);
};


}

#endif // MYTHOS_VNOVEL_H
