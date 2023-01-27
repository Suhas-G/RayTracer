#ifndef CG1RAYTRACER_MATERIALS_CONDUCTOR_HEADER
#define CG1RAYTRACER_MATERIALS_CONDUCTOR_HEADER

#include <rt/materials/material.h>

namespace rt {

/// A fancy perfect mirror or metal in general
class ConductorMaterial : public Material {
public:
    ConductorMaterial(float eta, float kappa);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const;
};

}

#endif