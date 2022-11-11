#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY): 
    center(center), forward(forward.normalize()), up(up.normalize()), scaleX(scaleX), scaleY(scaleY) {
    this->left = rt::cross(up, forward).normalize();
    this->up = rt::cross(forward, this->left);
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
    return Ray(((((-x * scaleX / 2) * left) + ((-y * scaleY / 2) * up)) + center), forward.normalize());
}

}

