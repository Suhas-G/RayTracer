#include <rt/cameras/perspective.h>
#include <cmath>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle):
    center(center), forward(forward.normalize()), up(up.normalize())
{
    this->left = rt::cross(up, forward).normalize();
    this->up = rt::cross(forward, this->left);
    this->tangentHorizontalOpeningAngle = std::tan(horizontalOpeningAngle / 2);
    this->tangentVerticalOpeningAngle = std::tan(verticalOpeningAngle / 2);
    rt_assert(verticalOpeningAngle == horizontalOpeningAngle);

}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    float f = (1/tangentHorizontalOpeningAngle);
    Vector direction(((forward * f) + (-x * left) + (-y * up)));
    return Ray(center, direction.normalize());
}

}
