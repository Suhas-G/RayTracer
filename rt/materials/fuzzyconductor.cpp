#include <rt/materials/fuzzyconductor.h>
#include <cmath>
#include <core/scalar.h>
#include <core/point.h>
#include <core/random.h>

namespace rt {

FuzzyConductorMaterial::FuzzyConductorMaterial(float eta, float kappa, float fuzzyangle)
: eta(eta), kappa(kappa), fuzzyangleTan(std::tan(fuzzyangle))
{
    etaPlusKappaSqr = (this->eta * this->eta) + (kappa * kappa);
}

RGBColor FuzzyConductorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(outDir);
    /* TODO */ 

    return RGBColor::rep(0.0f);

}

RGBColor FuzzyConductorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */
    return RGBColor::rep(0.0f);
}

static float fuzzyConductorFresnel(const Vector& normal, const Vector& reflected, float etaPlusKappaSqr, float eta) {
    float cos = rt::dot(normal.normalize(), reflected);

    float rParallelSqr = (etaPlusKappaSqr * rt::sqr(cos) - (2 * eta * cos) + 1) / (etaPlusKappaSqr * rt::sqr(cos) + (2 * eta * cos) + 1);
    float rPerpSqr = (etaPlusKappaSqr - (2 * eta * cos) + rt::sqr(cos)) / (etaPlusKappaSqr + (2 * eta * cos) + rt::sqr(cos));

    float fr = 0.5f * (rParallelSqr + rPerpSqr);
    return fr;
}

Material::SampleReflectance FuzzyConductorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {

    float cos = rt::dot(normal.normalize(), outDir.normalize());
    Vector _normal = normal;

    if (cos < 0) {
        cos = -cos;
        _normal = -normal;
    }

    Vector RI = (-outDir + 2 * (cos)*_normal).normalize();

    Vector discX = rt::cross(RI, _normal).normalize();
    Vector discY = rt::cross(RI, discX).normalize();
    float rho = std::sqrt(rt::random()) * fuzzyangleTan;
    float theta = rt::random(0.0f, 2 * rt::pi);
    float x = rho * std::cos(theta);
    float y = rho * std::sin(theta);

    Point discCenter = Point(texPoint.x + RI.x, texPoint.y + RI.y, texPoint.z + RI.z);
    Point p = discCenter + discX * x + discY * y;


    Vector fuzzyReflected = (p - texPoint).normalize();

    float fr = fuzzyConductorFresnel(_normal, fuzzyReflected, etaPlusKappaSqr, eta);

    return SampleReflectance{fuzzyReflected, RGBColor::rep(fr)};
}

Material::Sampling FuzzyConductorMaterial::useSampling() const {
    return rt::Material::SAMPLING_ALL;
}

}
