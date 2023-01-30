#include <rt/materials/transmission.h>
#include <math.h>
#include <core/random.h>
#include <core/scalar.h>
#include <tuple>
namespace rt {

    TransmissionMaterial::TransmissionMaterial(float alpha, RGBColor intensity)
        : alpha(alpha), intensity(intensity)
    {

    }

    RGBColor TransmissionMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        CG_UNUSED(inDir);

        return intensity;
    }

    RGBColor TransmissionMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        return RGBColor::rep(0.0f);
    }

    Material::SampleReflectance TransmissionMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {

        /* TODO */
        return Material::SampleReflectance(-outDir, RGBColor::rep(1.0f));

    }

    Material::Sampling TransmissionMaterial::useSampling() const {
        /* TODO */
        if (rt::random() < alpha) {
            return rt::Material::SAMPLING_NOT_NEEDED;
        } else {
            return rt::Material::SAMPLING_ALL;
        }
        
    }

    

}