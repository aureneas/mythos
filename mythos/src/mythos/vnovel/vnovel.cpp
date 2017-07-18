#include "../../../include/mythos/vnovel/vnovel.h"

namespace vnovel {


struct JointInstruction: public Instruction {
    InstructionList instruction;

    Animation* execute(Scene* scene) {
        JointAnimation* a = new JointAnimation();
        for (InstructionList::iterator it = instruction.begin(); it != instruction.end(); ++it)
            a->insert_animation((*it)->execute(scene));
        return a;
    }
};

struct SceneTransitionInstruction: public Instruction {
    SceneData* data;

    Animation* execute(Scene* scene) {
        return new MemberFunctionAnimation<void, Scene, SceneData*>(Scene::change_scene, scene, data);
    }
};

struct FadeInstruction: public Instruction {
    unsigned id;
    unsigned speed;
    ALLEGRO_COLOR fade;

    Animation* execute(Scene* scene) {
        Character* c = scene->get(id);
        return (c ? new ColorAnimation(color_linear_fade_animation, speed, &c->tint, fade) : nullptr);
    }
};

struct PositionInstruction: public Instruction {
    unsigned id;
    Position pos;

    Animation* execute(Scene* scene) {
        return new MemberFunctionAnimation<void, Scene, unsigned, Position>(Scene::change_position, scene, id, pos);
    }
};

struct SpriteInstruction: public Instruction {
    unsigned id;
    ALLEGRO_BITMAP* bmp;

    Animation* execute(Scene* scene) {
        Character* c = scene->get(id);
        return (c ? new ReplaceAnimation<ALLEGRO_BITMAP*>(&c->bmp, bmp) : nullptr);
    }
};

struct TextInstruction: public Instruction {
    ALLEGRO_USTR* name;
    ALLEGRO_USTR* text;

    Animation* execute(Scene* scene) {
        ChainAnimation* a = new ChainAnimation();
        a->insert_animation(new ReplaceAnimation<ALLEGRO_USTR*>(&scene->name, name));
        a->insert_animation(new FunctionAnimation<void, ALLEGRO_USTR*>(al_ustr_free, scene->text));
        a->insert_animation(new ReplaceAnimation<ALLEGRO_USTR*>(&scene->text, al_ustr_dup(text)));
        return a;
    }
};

struct InlineTextInstruction: public Instruction {
    ALLEGRO_USTR* text;

    Animation* execute(Scene* scene) {
        return new FunctionAnimation<bool, ALLEGRO_USTR*, const ALLEGRO_USTR*>(al_ustr_append, scene->text, text);
    }
};

struct ChoiceInstruction: public Instruction {
    std::list<Choice> options;

    Animation* execute(Scene* scene) {
        return new MemberFunctionAnimation<void, std::list<Choice>, std::list<Choice>::iterator, std::list<Choice>::iterator>(std::list<Choice>::assign, &scene->options, options.begin(), options.end());
    }
};


Animation* Scene::advance() {
    if (place == instruction->end())
        return nullptr;
    return (*place)->execute(this);
}

Character* Scene::get(unsigned id) {
    for (int i = 3; i >= 0; --i) {
        for (std::list<Character>::iterator it = chara[i].begin(); it != chara[i].end(); ++it) {
            if (it->id == id)
                return &*it;
        }
    }
    return nullptr;
}

void Scene::change_position(unsigned id, Position pos) {
    for (int i = 3; i >= 0; --i) {
        for (std::list<Character>::iterator it = chara[i].begin(); it != chara[i].end(); ++it) {
            if (it->id == id) {
                if ((Position)i != pos) {
                    Character c = *it;
                    chara[i].erase(it);
                    chara[pos].push_back(c);
                }
                return;
            }
        }
    }
}

void Scene::change_scene(SceneData* data) {
    for (int i = 2; i >= 0; --i)
        chara[i].clear();
    chara[OFFSCREEN].assign(data->chara.begin(), data->chara.end());
    al_ustr_free(text);
    instruction = &data->instruction;
    place = data->instruction.begin();
}


}
