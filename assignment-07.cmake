cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
    rt/lights/arealight.cpp
    rt/lights/arealight.h
    rt/integrators/recraytrace.cpp
    rt/integrators/recraytrace.h
    rt/materials/fuzzyconductor.cpp
    rt/materials/fuzzyconductor.h
    rt/materials/dielectric.cpp
    rt/materials/dielectric.h
    rt/materials/conductor.cpp
    rt/materials/conductor.h
    rt/cameras/dofperspective.cpp
    rt/cameras/dofperspective.h
    rt/solids/striangle.cpp
    rt/solids/striangle.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_distributed.cpp
    main/a_smooth.cpp
)
