cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
    rt/materials/dummy.cpp
    rt/lights/directional.cpp
    rt/lights/pointlight.cpp
    rt/lights/spotlight.cpp
    rt/integrators/raytrace.cpp
)

set(ASSIGNMENT_SOURCES
    rt/materials/material.h
    rt/materials/dummy.h
    rt/materials/lambertian.cpp
    rt/materials/lambertian.h
    rt/materials/phong.cpp
    rt/materials/phong.h
    rt/lights/light.h
    rt/lights/pointlight.h
    rt/lights/directional.h
    rt/lights/spotlight.h
    rt/textures/texture.h
    rt/textures/constant.cpp
    rt/textures/constant.h
    rt/integrators/raytrace.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_materials.cpp
    main/a_lighting.cpp
)
