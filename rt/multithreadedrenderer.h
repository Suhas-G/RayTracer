#ifndef CG1RAYTRACER_MULTITHREADEDRENDERER_HEADER
#define CG1RAYTRACER_MULTITHREADEDRENDERER_HEADER

#include <core/scalar.h>

namespace rt {

class Image;
class Camera;
class Integrator;

class MultiThreadedRenderer {
public:
    MultiThreadedRenderer(Camera* cam, Integrator* integrator, int maxThreads = 0);
    void setSamples(uint samples) { this->samples = samples; }
    void render(Image& img);

private:
    Camera* cam;
    Integrator* integrator;
    uint samples;
    int maxThreads = 0;

    void renderWithMultipleSamples(Image& img);
};

}

#endif