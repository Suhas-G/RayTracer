#ifndef CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class EnvironmentCamera : public Camera {
public:
    EnvironmentCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float fieldOfViewX,
        float fieldOfViewY
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    Point center;
    Vector forward;
    Vector up;
    Vector left;
    float fieldOfViewX;
    float fieldOfViewY;
};

}


#endif