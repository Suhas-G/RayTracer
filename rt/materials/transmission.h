#ifndef CG1RAYTRACER_MATERIALS_TRANSMISSION_HEADER
#define CG1RAYTRACER_MATERIALS_TRANSMISSION_HEADER

#include <rt/materials/material.h>

namespace rt {

/// A fancy perfect glass (eta = ~1.55) or dielectric in general
class TransmissionMaterial : public Material {
public:
    TransmissionMaterial(float alpha, RGBColor intensity);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const;
private:
    float alpha;
    RGBColor intensity;
};

}

#endif