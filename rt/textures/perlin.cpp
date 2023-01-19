#include <vector>
#include <cmath>
#include <tuple>
#include <core/point.h>
#include <rt/textures/perlin.h>
#include <core/interpolate.h>
namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
: white(white), black(black){
}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
    

    float total = 0;
    for (auto& tuple : frequency_amplitude) {
        float frequency = std::get<0>(tuple);
        float amplitude = std::get<1>(tuple);
        float x = std::floor(coord.x * frequency);
        float y = std::floor(coord.y * frequency);
        float z = std::floor(coord.z * frequency);
        float xWeight = coord.x * frequency - x;
        xWeight = xWeight * xWeight * (3 - 2 * xWeight);
        float yWeight = coord.y * frequency - y;
        yWeight = yWeight * yWeight * (3 - 2 * yWeight);
        float zWeight = coord.z * frequency - z;
        zWeight = zWeight * zWeight * (3 - 2 * zWeight);
        total += amplitude * lerp3d(
            noise(x, y, z), noise(x + 1, y, z), noise(x, y + 1, z), noise(x + 1, y + 1, z),
            noise(x, y, z + 1), noise(x + 1, y, z + 1), noise(x, y + 1, z + 1), noise(x + 1, y + 1, z + 1),
            xWeight, yWeight, zWeight
        );
    }
    return lerp(black, white, (total + 1) / 2.0f);
}

rt::RGBColor PerlinTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);

    /* TODO */ NOT_IMPLEMENTED;
}

rt::RGBColor PerlinTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);

    /* TODO */ NOT_IMPLEMENTED;
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    frequency_amplitude.push_back(std::make_tuple(frequency, amplitude));
}

}