#include <rt/solids/solid.h>
#include <rt/coordmappers/world.h>

namespace rt {

Solid::Solid(CoordMapper* texMapper, Material* material)
{
    if (texMapper == nullptr) {
        texMapper = new WorldMapper();
    }
    this->setCoordMapper(texMapper);
    this->setMaterial(material);
}

}
