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

void AreaWrapper::push(Area* a, int k1_a, int k2_a) {
    a->angle = angle;
    a->sort();

    Vec2 nrangle( angle.y, -angle.x );

    for (int i = (int)a->size - 1; i >= 0; --i) {
        for (int j = (int)a->size - 1; j >= 0; --j) {
            Vec2 p( i * a->width, j * a->width );
            int k1 = k1_a - (p * angle);
            int k2 = k2_a + (p * nrangle);
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
        int k1 = k1_a - (f->vol->crd * angle) - f->vol->crd.z;
        int k2 = k2_a + (f->vol->crd * nrangle);
        //std::cout << "(" << k1 << ", " << k2 << ") -> " << f << "\n";
        AWFormList::iterator it1 = form.find(k1);
        if (it1 == form.end())
            form.emplace_hint(it1, k1, AWInnerFormList({{k2, f}}));
        else
            it1->second.emplace_hint(it1->second.end(), k2, f);
    }
    //std::cout << "\n";
}

void AreaWrapper::reset() {
    tile.clear();
    form.clear();

    Vec2 angle = area->angle;
    Vec2 nrangle( angle.y, -angle.x );

    if (area)   push(area, 0, 0);
    for (int i = 7; i >= 0; --i) {
        /*Vec2 q = direction_to_point((Direction)i);
        q *= (int)(prox[i]->size * prox[i]->width);
        if (prox[i])    push(prox[i],
                             -(angle * q),
                             (nrangle * q));*/
    }
}
