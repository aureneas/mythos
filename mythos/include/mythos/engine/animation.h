#ifndef MYTHOS_ANIMATION_H
#define MYTHOS_ANIMATION_H

#include <forward_list>
#include <list>
#include <memory>

#include <allegro5/allegro.h>

#include "../utility/point.h"
#include "../utility/random.h"

struct Animation {
    /**
     *  Updates <this> each frame.
     *  @return     true if <this> is complete, false otherwise
     */
    virtual bool update_frame() {
        return true;
    }
};

typedef std::list<std::unique_ptr<Animation> >      AnimationList;

/**
 *  Simultaneously performs all animations.
 */
struct JointAnimation: public Animation {
    AnimationList anim_list;

    virtual bool update_frame();
    void insert_animation(Animation*);
};

/**
 *  Performs multiple animations in sequence.
 */
struct ChainAnimation: public JointAnimation {
    bool update_frame();
};

typedef void (*AnimationFunction)(Animation*);

/**
 *  Used to change other values over time.
 */
struct SetDurationAnimation: public Animation {
    AnimationFunction afunc;

    unsigned duration;
    unsigned frame;

    SetDurationAnimation(AnimationFunction, unsigned);
    bool update_frame();
};

/**
 *  Animation that works with Point.
 */
struct MoveAnimation: public SetDurationAnimation {
    Point* point;
    Point target;

    MoveAnimation(AnimationFunction, unsigned, Point*, Point);
};

/**
 *  Animation that shakes a Point randomly.
 */
struct ShakeAnimation: public MoveAnimation {
    Point mag;

    ShakeAnimation(AnimationFunction, unsigned, Point*, Point, Point);
};

/**
 *  Animation that works with ALLEGRO_COLOR.
 */
struct ColorAnimation: public SetDurationAnimation {
    ALLEGRO_COLOR* color;   // pointer to the ALLEGRO_COLOR to be altered
    ALLEGRO_COLOR target;   // ALLEGRO_COLOR that will be the end product of the animation
    float delta[4];         // color changes from initial to target

    ColorAnimation(AnimationFunction, unsigned, ALLEGRO_COLOR*, ALLEGRO_COLOR);
};

void color_linear_fade_animation(Animation*);

/**
 *  Animation that gradually prints an ALLEGRO_USTR string.
 */
struct TextAnimation: public Animation {
    ALLEGRO_USTR* target;   // string to print to
    ALLEGRO_USTR* text;     // string with text to print
    unsigned speed;         // characters printed per second

    TextAnimation(unsigned, ALLEGRO_USTR*, ALLEGRO_USTR*);
    ~TextAnimation();
    bool update_frame();
};

/**
 *  Animation that animates a bitmap.
 */
template <typename T>
struct ReplaceAnimation: public Animation {
    unsigned frame;
    T* t;
    std::forward_list<std::pair<unsigned, T> > t_stack;

    ReplaceAnimation(T* pt) {
        t = pt;
        frame = 0;
    }
    ReplaceAnimation(T* pt, T tt) {
        t = pt;
        frame = 0;
        t_stack.emplace_front(1, tt);
    }
    bool update_frame() {
        if (frame++ == 0)
            *t = t_stack.front().second;
        if (frame == t_stack.front().first) {
            t_stack.pop_front();
            frame = 0;
        }
        return t_stack.empty();
    }
    void insert(unsigned s, T tt) {
        t_stack.emplace_front(s, tt);
    }
};

/**
 *  Animation that calls a function.
 */
template <typename Ret, typename ... Params>
struct FunctionAnimation: public Animation {
    std::function<Ret ()> func;

    FunctionAnimation(Ret (*f)(Params...), Params ... p) {
        func = std::bind(f, p...);
    }
    bool update_frame() {
        func();
        return true;
    }
};

/**
 *  Animation that calls a member function.
 */
template <typename Ret, typename T, typename ... Params>
struct MemberFunctionAnimation: public Animation {
    std::function<Ret (T*)> func;
    T* t;

    MemberFunctionAnimation(Ret (T::*f)(Params...), T* pt, Params... p) {
        func = std::bind(f, std::placeholders::_1, p...);
        t = pt;
    }
    bool update_frame() {
        func(t);
        return true;
    }
};


#endif // MYTHOS_ANIMATION_H
