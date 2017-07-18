#include "../../include/mythos/animation.h"


bool JointAnimation::update_frame() {
    for (AnimationList::iterator it = anim_list.begin(); it != anim_list.end(); ++it) {
        if ((*it)->update_frame())
            anim_list.erase(it--);
    }
    return anim_list.empty();
}

void JointAnimation::insert_animation(Animation* a) {
    anim_list.emplace_back(a);
}

bool ChainAnimation::update_frame() {
    while (!anim_list.empty() && anim_list.front()->update_frame())
        anim_list.pop_front();
    return anim_list.empty();
}


SetDurationAnimation::SetDurationAnimation(AnimationFunction f, unsigned d) {
    afunc = f;
    duration = d;
    frame = 0;
}

bool SetDurationAnimation::update_frame() {
    if (afunc)  afunc(this);
    return (++frame >= duration);
}

MoveAnimation::MoveAnimation(AnimationFunction f, unsigned d, Point* p, Point t) : SetDurationAnimation(f, d) {
    point = p;
    target = t;
}

ShakeAnimation::ShakeAnimation(AnimationFunction f, unsigned d, Point* p, Point t, Point m) : MoveAnimation(f, d, p, t) {
    mag = m;
}

ColorAnimation::ColorAnimation(AnimationFunction f, unsigned d, ALLEGRO_COLOR* pcol, ALLEGRO_COLOR trg) :
        SetDurationAnimation(f, d) {
    color = pcol;
    target = trg;
}

/**
 *  Performs a shake animation. Built for ShakeAnimation.
 */
void point_shake_animation(Animation* anim) {
    ShakeAnimation* a = dynamic_cast<ShakeAnimation*>(anim);
    if (a->frame + 1 >= a->duration)
        *a->point = a->target;
    else
        *a->point = Point{ a->target.x + random<int>(-a->mag.x, a->mag.x), a->target.y + random<int>(-a->mag.y, a->mag.y) };
}

/**
 *  Performs a linear fade. Built for ColorAnimation.
 */
void color_linear_fade_animation(Animation* anim) {
    ColorAnimation* a = dynamic_cast<ColorAnimation*>(anim);
    float ca[4];
    al_unmap_rgba_f(*a->color, ca, ca + 1, ca + 2, ca + 3);
    // set deltas (on first frame)
    if (a->frame == 0) {
        float cb[4];
        al_unmap_rgba_f(a->target, cb, cb + 1, cb + 2, cb + 3);
        for (int i = 3; i >= 0; --i)
            a->delta[i] = cb[i] - ca[i];
    }
    // change color
    for (int i = 3; i >= 0; --i)
        ca[i] += a->delta[i] / a->duration;
    *a->color = al_map_rgba_f(ca[0], ca[1], ca[2], ca[3]);
}


TextAnimation::TextAnimation(unsigned s, ALLEGRO_USTR* trg, ALLEGRO_USTR* t) {
    speed = s;
    target = trg;
    text = al_ustr_dup(t);
}

TextAnimation::~TextAnimation() {
    al_ustr_free(text);
}

bool TextAnimation::update_frame() {
    al_ustr_append_chr(target, al_ustr_get(text, 0));
    al_ustr_remove_chr(text, 0);
    return (al_ustr_size(text) == 0);
}
