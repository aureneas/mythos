#include <algorithm>

#include "../../../include/mythos/form/form.h"


static Point comp_angle;

/**
 *  Compares the locations of two forms relative to angle of view.
 *  @return     true if pf1 is behind pf2 (relative to active Container), false otherwise
 */
bool compare(const std::unique_ptr<Form>& pf1, const std::unique_ptr<Form>& pf2) {
    Point pos1 = { pf1->vol->crd.x, pf1->vol->crd.y };
    Point pos2 = { pf2->vol->crd.x, pf2->vol->crd.y };
    if (comp_angle.x == 0)   return ((comp_angle.y < 0) ? (pos1.y < pos2.y) : (pos1.y > pos2.y));
    if (comp_angle.y == 0)   return ((comp_angle.x < 0) ? (pos1.x < pos2.x) : (pos1.x > pos2.x));
    int t1 = (pos1 * comp_angle);
    int t2 = (pos2 * comp_angle);
    if (t1 != t2)       return (t1 > t2);
    return (pf1->vol->crd.z < pf2->vol->crd.z);
}


void Container::sort() {
    comp_angle = angle;
    std::sort(child.begin(), child.end(), compare);
}

void Container::insert(Form* form) {
    child.emplace_back(form);
    form->parent = this;
    sort();
}

void Container::remove(Form* form) {
    for (ContainerFormList::iterator it = child.begin(); it != child.end(); ++it) {
        if (it->get() == form) {
            child.erase(it);
            return;
        }
    }
}

void Container::transfer(Container* cont, Form* form) {
    for (ContainerFormList::iterator it = child.begin(); it != child.end(); ++it) {
        if (it->get() == form) {
            cont->insert(it->release());
            child.erase(it);
            return;
        }
    }
}

void Container::set_position(Form* form, Vec3 crd) {
	Volume* v = form->vol;
	Vec3 crd_old = v->crd;
	v->crd = crd;

}
