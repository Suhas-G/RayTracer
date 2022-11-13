#include <rt/cameras/environment.h>
#include <cmath>

namespace rt {

EnvironmentCamera::EnvironmentCamera(const Point& center, const Vector& forward, const Vector& up, float fieldOfViewX, float fieldOfViewY):
    center(center), forward(forward.normalize()), up(up.normalize()), fieldOfViewX(fieldOfViewX), fieldOfViewY(fieldOfViewY)
{
    this->left = rt::cross(this->up, this->forward).normalize();
    this->up = rt::cross(this->forward, this->left);
}

Ray EnvironmentCamera::getPrimaryRay(float x, float y) const {


    float phi = x * fieldOfViewX / 2;
    float theta = y * fieldOfViewY / 2;

    float cos_theta = std::cos(theta);
    x = cos_theta * std::cos(phi);
    y = cos_theta * std::sin(phi);
    float z = std::sin(theta);
    Vector direction(((z * forward) + (-x * left) + (y * up)));
    return Ray(center, direction.normalize());
}

}