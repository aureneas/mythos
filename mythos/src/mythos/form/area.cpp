#include "../../../include/mythos/form/area.h"
#include <iostream> // TODO remove later

AreaWrapper::AreaWrapper() {
    area = nullptr;
    for (int i = 7; i >= 0; --i)
        prox[i] = nullptr;
}

AreaWrapper::AreaWrapper(Area* a) : AreaWrapper() {
    set_area(a);
}

void AreaWrapper::set_area(Area* a) {
    if (area != a) {
        for (int i = 7; i >= 0; --i) {
            if (prox[i] == a) {
                // TODO
                area = a;
                reset();
                return;
            }
        }

        // TODO
        area = a;
        reset();
    }
}

void AreaWrapper::push(Area* a) {
    Point angle = area->angle;
    Point nrangle = { angle.y, -angle.x };

    for (int i = (int)a->size - 1; i >= 0; --i) {
        for (int j = (int)a->size - 1; j >= 0; --j) {
            Point p = { i * a->width, j * a->width };
            int k1 = -(p * angle);
            int k2 = (p * nrangle);
            //std::cout << "(" << i << ", " << j << ") -> (" << k1 << ", " << k2 << ") -> " << &a->tile[i][j] << "\n";
            AWTileList::iterator it1 = tile.find(k1);
            if (it1 == tile.end())
                tile.emplace(k1, AWInnerTileList({{k2, &a->tile[i][j]}}));
            else
                it1->second.emplace(k2, &a->tile[i][j]);
        }
    }
    //std::cout << "\n";

    for (ContainerFormList::iterator it = a->child.begin(); it != a->child.end(); ++it) {
        Form* f = it->get();
        int k1 = -(f->vol.base.center * angle) - f->vol.z;
        int k2 = f->vol.base.center * nrangle;
        //std::cout << "(" << k1 << ", " << k2 << ") -> " << f << "\n";
        AWFormList::iterator it1 = form.find(k1);
        if (it1 == form.end())
            form.emplace(k1, AWInnerFormList({{k2, f}}));
        else
            it1->second.emplace(k2, f);
    }
    //std::cout << "\n";
}

void AreaWrapper::reset() {
    tile.clear();
    form.clear();

    if (area)   push(area);
    for (int i = 7; i >= 0; --i)
        if (prox[i])    push(prox[i]);
}
