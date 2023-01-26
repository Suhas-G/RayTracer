#include <rt/materials/mirror.h>
#include <cmath>
#include <core/scalar.h>
#include <core/point.h>
#include <core/random.h>

namespace rt {

MirrorMaterial::MirrorMaterial(float eta, float kappa)
: eta(eta), kappa(kappa)
{
    etaPlusKappaSqr = (this->eta * this->eta) + (kappa * kappa);
}

RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(outDir);

    return RGBColor::rep(0.0f);

}

RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */
    return RGBColor::rep(0.0f);
}

static float mirrorFresnel(const Vector& normal, const Vector& reflected, float etaPlusKappaSqr, float eta) {
    float cos = rt::dot(normal.normalize(), reflected);

    float rParallelSqr = (etaPlusKappaSqr * rt::sqr(cos) - (2 * eta * cos) + 1) / (etaPlusKappaSqr * rt::sqr(cos) + (2 * eta * cos) + 1);
    float rPerpSqr = (etaPlusKappaSqr - (2 * eta * cos) + rt::sqr(cos)) / (etaPlusKappaSqr + (2 * eta * cos) + rt::sqr(cos));

    float fr = 0.5f * (rParallelSqr + rPerpSqr);
    return fr;
}

Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {

    float cos = rt::dot(normal.normalize(), outDir.normalize());
    Vector _normal = normal;

    if (cos < 0) {
        cos = -cos;
        _normal = -normal;
    }

    Vector RI = (-outDir + 2 * (cos)*_normal).normalize();

    float fr = mirrorFresnel(_normal, RI, etaPlusKappaSqr, eta);

    return SampleReflectance{RI, RGBColor::rep(fr)};
}

Material::Sampling MirrorMaterial::useSampling() const {
    return rt::Material::SAMPLING_ALL;
}

}