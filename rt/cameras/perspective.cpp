#include <rt/cameras/perspective.h>
#include <cmath>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle):
    center(center), forward(forward.normalize()), up(up.normalize())
{
    this->left = rt::cross(up, forward).normalize();
    this->up = rt::cross(forward, this->left).normalize();
    float tangentHorizontalOpeningAngle = std::tan(horizontalOpeningAngle / 2);
    float tangentVerticalOpeningAngle = std::tan(verticalOpeningAngle / 2);
    this->focalLength = (1/tangentVerticalOpeningAngle);
    this->aspectRatio = tangentHorizontalOpeningAngle * this->focalLength;
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector direction(((forward * focalLength) + ((-x * aspectRatio) * left) + (y * up)));
    return Ray(center, direction.normalize());
}

}
