#include <core/scalar.h>
#include <core/image.h>
#include <core/random.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <rt/cameras/camera.h>
#include <rt/integrators/integrator.h>
#include <iostream>

namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{}

void Renderer::render(Image& img) {
    CG_UNUSED(img);
    /* TODO */ 
    const auto width = img.width();
    const auto height = img.height();
    float ndcx, ndcy, sscx, sscy;

    rt::uint verticalSamples = static_cast<rt::uint>(std::floor(std::sqrt(samples)));
    rt::uint horizontalSamples = verticalSamples + ((samples - (verticalSamples * verticalSamples)) / verticalSamples);
    float sampleHeight = 1.0f / verticalSamples;
    float sampleWidth = 1.0f / horizontalSamples;

    for (rt::uint x = 0; x < width; x++) {
        for (rt::uint y = 0; y < height; y++) {
            if (samples == 1) {
                ndcx = (x + 0.5f) / width;
                ndcy = (y + 0.5f) / height;
                sscx = (ndcx * 2) - 1;
                sscy = (ndcy * 2) - 1;
                Ray ray = cam->getPrimaryRay(sscx, sscy);
                img(x, height - y - 1, integrator->getRadiance(ray));
            } else {
                
                RGBColor color = RGBColor::rep(0.0f);
                for (rt::uint i = 0; i < horizontalSamples; i++) {
                    for (rt::uint j = 0; j < verticalSamples; j++) {
                        float xOffset = (i * sampleWidth) + (rt::random() * sampleWidth);
                        float yOffset = (j * sampleHeight) + (rt::random() * sampleHeight);
                        ndcx = (x + xOffset) / width;
                        ndcy = (y + yOffset) / height;
                        sscx = (ndcx * 2) - 1;
                        sscy = (ndcy * 2) - 1;
                        Ray ray = cam->getPrimaryRay(sscx, sscy);
                        color = color + (1.0f/ (horizontalSamples * verticalSamples)) * integrator->getRadiance(ray);
                    }
                }
                img(x, height - y - 1, color);
            }
        }
    }
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
            img(x, height - y - 1, a2computeColor(cam->getPrimaryRay(sscx, sscy)));
        }
    }
}

}
