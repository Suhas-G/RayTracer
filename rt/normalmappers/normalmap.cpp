#include <rt/normalmappers/normalmap.h>


namespace rt {
    Vector NormalMapper::getNormal(const Intersection& hit) {
        Vector perturbation = this->computePerturbation(hit);

        Vector r = hit.normal();
        float rx = std::abs(r.x), ry = std::abs(r.y), rz = std::abs(r.z);
        Vector s = Vector::rep(0);
        if (rx <= ry && rx <= rz) {
            s.y = -r.z;
            s.z = r.y;
        } else if (ry <= rx && ry <= rz) {
            s.x = -r.z;
            s.z = r.x;
        } else {
            s.x = -r.y;
            s.y = r.x;
        }
        s = s.normalize();
        Vector t = rt::cross(r, s);

        Vector perturbedNormal = r + (r * perturbation.x) + (s * perturbation.y) + (t * perturbation.z);
        return perturbedNormal.normalize();
    }
}