#include <rt/solids/solid.h>
#include <rt/coordmappers/world.h>

namespace rt {

Solid::Solid(CoordMapper* texMapper, Material* material)
{
    /* TODO */
    // FIXME: Apparently commenting out this is a must....LOL
    // if (this->texMapper == nullptr) {
    //     texMapper = new WorldMapper();
    // }
    this->texMapper = texMapper;
    this->material = material;
}

}
