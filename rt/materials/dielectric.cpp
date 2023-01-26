#include <rt/materials/dielectric.h>
#include <math.h>
#include <core/random.h>
#include <core/scalar.h>
#include <tuple>
namespace rt {

    DielectricMaterial::DielectricMaterial(float eta)
        : eta_t(eta)
    {
        this->eta_r = this->eta_i / this->eta_t;
    }

    RGBColor DielectricMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        CG_UNUSED(inDir);

        return RGBColor::rep(0.0f);
    }

    RGBColor DielectricMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        CG_UNUSED(texPoint);
        CG_UNUSED(normal);
        CG_UNUSED(outDir);
        return RGBColor::rep(0.0f);
    }

    std::tuple<float, float> dielectricFresnel(float _eta_i, float _eta_t, float cos, float cos_t) {
        float rParallel = (_eta_t * cos - _eta_i * cos_t) / (_eta_t * cos + _eta_i * cos_t);
        float rPerp = (_eta_i * cos - _eta_t * cos_t) / (_eta_i * cos + _eta_t * cos_t);

        return std::make_tuple(rParallel, rPerp);
    }

    Material::SampleReflectance DielectricMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {

        /* TODO */
        float _eta = this->eta_r;
        float _eta_t = this->eta_t;
        float _eta_i = this->eta_i;
        float cos = rt::dot(normal.normalize(), outDir.normalize());
        float sin = std::sqrt(1 - (cos * cos));
        Vector _normal = normal;

        if (cos < 0) {
            _eta = 1.0f / _eta;
            _eta_i = this->eta_t;
            _eta_t = this->eta_i;
            cos = -cos;
            _normal = -normal;
        }

        Vector RI = (-outDir + 2 * (cos)*_normal).normalize();
        float cos_t = rt::dot(_normal, RI);
        
        float rParallel = 0;
        float rPerp = 0;

        std::tie(rParallel, rPerp) = dielectricFresnel(_eta_i, _eta_t, cos, cos_t);

        float fr = 0.5f * (rt::sqr(rParallel) + rt::sqr(rPerp));

        if (sin > (_eta_t / _eta_i)) {
            return SampleReflectance(RI, RGBColor::rep(1.0f));
        } else if (rt::random() <= 0.2) {
            return SampleReflectance(RI, RGBColor::rep(fr));
        }else {
            Vector refracted = ((_eta * cos) - std::sqrt(1 - (sin * sin) * (_eta * _eta))) * _normal - _eta * outDir;
            return SampleReflectance(refracted.normalize(), RGBColor::rep((1 - fr) * rt::sqr(_eta)));
        }

    }

    Material::Sampling DielectricMaterial::useSampling() const {
        /* TODO */
        return rt::Material::SAMPLING_ALL;
    }

    

}