#include "../../../include/mythos/form/event.h"

bool FormIntersectCondition::trigger(Event* e) {
    return form->vol.intersect(&e->vol);
}


void Event::update_frame() {
    if (condition->trigger(this))
        execute();
}


void TransportationEvent::execute() {
    if (UnequalCondition<void*>* pcond = dynamic_cast<UnequalCondition<void*>*>(condition)) {
        Form* form = (Form*)pcond->last;
        form->parent->transfer(cont, form);
        form->vol.base.center = point;
        form->vol.z = z;
    }
}
