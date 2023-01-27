#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <cmath>
#include <core/random.h>
#include <core/scalar.h>

namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
    :center(center), forward(forward.normalize()), up(up.normalize()), distanceAlongRay(focalDistance), apertureRadius(apertureRadius)
{
    this->left = rt::cross(up, forward).normalize();
    this->up = rt::cross(forward, this->left).normalize();
    float tangentHorizontalOpeningAngle = std::tan(horizontalOpeningAngle / 2);
    float tangentVerticalOpeningAngle = std::tan(verticalOpeningAngle / 2);
    this->focalLength = (1/tangentVerticalOpeningAngle);
    this->aspectRatio = tangentHorizontalOpeningAngle * this->focalLength;
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector direction(((forward * focalLength) + ((-x * aspectRatio) * left) + (y * up)));
    Ray originalRay = Ray(center, direction.normalize());
    Point p = originalRay.getPoint(this->distanceAlongRay);

    float rho = std::sqrt(rt::random()) * this->apertureRadius;
    float theta = rt::random(0, 2 * rt::pi);
    Point newCenter = this->center + (this->left * rho * std::cos(theta)) + (this->up * rho * std::sin(theta));

    Vector newDirection = p - newCenter;
    return Ray(newCenter, newDirection.normalize());
}

}