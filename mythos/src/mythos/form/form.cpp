#include <algorithm>

#include "../../../include/mythos/form/form.h"


static Point comp_angle;

/**
 *  Compares the locations of two forms relative to angle of view.
 *  @return     true if pf1 is behind pf2 (relative to active Container), false otherwise
 */
bool compare(const std::unique_ptr<Form>& pf1, const std::unique_ptr<Form>& pf2) {
    Point pos1 = pf1->vol.base.center;
    Point pos2 = pf2->vol.base.center;
    if (comp_angle.x == 0)   return ((comp_angle.y < 0) ? (pos1.y < pos2.y) : (pos1.y > pos2.y));
    if (comp_angle.y == 0)   return ((comp_angle.x < 0) ? (pos1.x < pos2.x) : (pos1.x > pos2.x));
    int t1 = (pos1.x * comp_angle.x) + (pos1.y * comp_angle.y);
    int t2 = (pos2.x * comp_angle.x) + (pos2.y * comp_angle.y);
    if (t1 != t2)       return (t1 > t2);
    return (pf1->vol.z < pf2->vol.z);
}


void Container::sort() {
    comp_angle = angle;
    std::sort(child.begin(), child.end(), compare);
}

void Container::transfer(Container* cont, Form* form) {
    for (ContainerFormList::iterator it = child.begin(); it != child.end(); ++it) {
        if (it->get() == form) {
            cont->child.emplace_back(it->release());
            cont->sort();
            child.erase(it);
            form->parent = cont;
            return;
        }
    }
}
