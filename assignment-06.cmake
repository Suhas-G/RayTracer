cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
    core/interpolate.cpp
    core/interpolate-impl.h
    core/interpolate.h
    core/random.cpp
    core/random.h
    rt/coordmappers/coordmapper.h
    rt/coordmappers/cylindrical.cpp
    rt/coordmappers/cylindrical.h
    rt/coordmappers/plane.cpp
    rt/coordmappers/plane.h
    rt/coordmappers/spherical.cpp
    rt/coordmappers/spherical.h
    rt/coordmappers/tmapper.cpp
    rt/coordmappers/tmapper.h
    rt/coordmappers/world.cpp
    rt/coordmappers/world.h
    rt/textures/imagetex.cpp
    rt/textures/imagetex.h
    rt/textures/checkerboard.cpp
    rt/textures/checkerboard.h
    rt/textures/perlin.cpp
    rt/textures/perlin.h
    rt/materials/flatmaterial.cpp
    rt/materials/flatmaterial.h
    rt/materials/combine.cpp
    rt/materials/combine.h
    rt/loaders/objmat.cpp
    ${MY_ADDITIONAL_SOURCES}
    main/a_local.cpp
    main/a_textures.cpp
    main/a_mappers.cpp
)

file(COPY
    models/stones_diffuse.png
    DESTINATION ${CMAKE_BINARY_DIR}/models
)
