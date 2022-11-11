#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <rt/cameras/camera.h>
#include <iostream>

namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{}

void Renderer::render(Image& img) {
    CG_UNUSED(img);
    /* TODO */ NOT_IMPLEMENTED;
}

}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

void Renderer::test_render1(Image& img) {
    const auto width = img.width();
    const auto height = img.height();
    for (rt::uint x = 0; x < width; x++) {
        for (rt::uint y = 0; y < height; y++) {
            img(x, y, a1computeColor(x, y, width, height));
        }
    }
}
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
    const auto width = img.width();
    const auto height = img.height();
    float ndcx, ndcy, sscx, sscy;
    for (rt::uint x = 0; x < width; x++) {
        for (rt::uint y = 0; y < height; y++) {
            ndcx = (x + 0.5f) / width;
            ndcy = (y + 0.5f) / height;
            sscx = (ndcx * 2) - 1;
            sscy = (ndcy * 2) - 1;
            img(x, y, a2computeColor(cam->getPrimaryRay(sscx, sscy)));
        }
    }
}

}
