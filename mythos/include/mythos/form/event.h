#ifndef MYTHOS_EVENT_H
#define MYTHOS_EVENT_H

#include "form.h"

struct Event;

struct EventCondition {
    virtual bool trigger(Event* e) { return false; }
};

template <typename T>
struct EqualCondition: public EventCondition {
    T* pbind;    // pointer to T which event is bound to
    T cond;      // triggers when pbind matches cond
    T last;      // last check of *pbind
    bool trigger(Event* e) {
            if (*pbind != last) {
                last = *pbind;
                return (cond == last);
            }
            return false;
        }
};

template <typename T>
struct UnequalCondition: public EventCondition {
    T* pbind;    // pointer to T which event is bound to
    T cond;      // triggers when pbind does not match cond
    T last;      // last check of *pbind
    bool trigger(Event* e) {
            if (*pbind != last) {
                if (last == cond) {
                    last = *pbind;
                    return true;
                }
                last = *pbind;
            }
            return false;
        }
};

struct FormIntersectCondition: public EventCondition {
    Form* form;
    bool trigger(Event*);
};


struct Event: public Form {
    EventCondition* condition;
    void update_frame();
    virtual void execute() = 0;
};

struct TransportationEvent: public Event {
    Container* cont;
    Point point;
    int z;
    void execute();
};


#endif // MYTHOS_EVENT_H
