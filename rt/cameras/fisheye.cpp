#include <rt/cameras/fisheye.h>
#include <cmath>

namespace rt {

FisheyeCamera::FisheyeCamera(const Point& center, const Vector& forward, const Vector& up, float fieldOfView):
    center(center), forward(forward.normalize()), up(up.normalize()), fieldOfView(fieldOfView)
{
    this->left = rt::cross(this->up, this->forward).normalize();
    this->up = rt::cross(this->forward, this->left);
}

Ray FisheyeCamera::getPrimaryRay(float x, float y) const {

    float r = std::sqrt(x*x + y*y);
    float phi = std::atan2(y, x);
    float theta = r * fieldOfView / 2;

    float sin_theta = std::sin(theta);
    x = sin_theta * std::cos(phi);
    y = sin_theta * std::sin(phi);
    float z = std::cos(theta);
    Vector direction(((z * forward) + (-x * left) + (y * up)));
    return Ray(center, direction.normalize());
}

}