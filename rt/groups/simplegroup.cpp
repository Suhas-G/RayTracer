#include <rt/groups/simplegroup.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
    /* TODO */
    BBox box = BBox::empty();
    for (auto p: primitives) {
        box.extend(p->getBounds());
    }
    return box;
}

Intersection SimpleGroup::intersect(const Ray& ray, float tmin, float tmax) const {
    Intersection groupIntersection = Intersection::failure();
    Intersection temp;

    for (auto p: primitives) {
        temp = p->intersect(ray, tmin, tmax);
        if (temp) {
            // rt_assert(temp.distance < tmax);
            tmax = temp.distance;
            groupIntersection = temp;
        }
    }
    return groupIntersection;
}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    primitives.push_back(p);
}

void SimpleGroup::setMaterial(Material* m) {
    for (auto p: primitives) {
        p->setMaterial(m);
    }
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    for (auto p: primitives) {
        p->setCoordMapper(cm);
    }
}

void SimpleGroup::setNormalMapper(NormalMapper* nm) {
    for (auto p: primitives) {
        p->setNormalMapper(nm);
    }
}

}
