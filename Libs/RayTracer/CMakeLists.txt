cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# RayTracer library ###########################################################
set( RT_SOURCES
    Include/Camera.h
    Include/Canvas.h
    Include/Color.h
    Include/Intersection.h
    Include/Lighting.h
    Include/Material.h
    Include/Matrix.h
    Include/Pattern.h
    Include/Plane.h
    Include/Ray.h
    Include/Shape.h
    Include/SimplexNoise.h
    Include/Sphere.h
    Include/Transformations.h
    Include/Tuple.h
    Include/Types.h
    Include/Util.h
    Include/World.h
    Src/Camera.cpp
    Src/Canvas.cpp
    Src/Intersection.cpp
    Src/Lighting.cpp
    Src/Material.cpp
    Src/Pattern.cpp
    Src/Plane.cpp
    Src/Ray.cpp
    Src/Shape.cpp
    Src/SimplexNoise.cpp
    Src/Sphere.cpp
    Src/Transformations.cpp
    Src/Tuple.cpp
    Src/World.cpp )

AddTarget( RayTracer LIBRARY EXPORT_HEADER
    FOLDER 01_Libs
    SOURCES ${RT_SOURCES}
    PUBLIC_DIRS "./Include"
    PRIVATE_DIRS "./Src"
    #PUBLIC_DEFS "USE_SSE" "_ENABLE_EXTENDED_ALIGNED_STORAGE" stuff for MathIntrinsics.h
    PRIVATE_DEFS "RENDER_TASKS=12" )

# RayTracer tests #############################################################
set( TEST_SOURCES
    Test/Src/Canvas.cpp
    Test/Src/Camera.cpp
    Test/Src/Color.cpp
    Test/Src/Intersection.cpp
    Test/Src/Light.cpp
    Test/Src/Main.cpp
    Test/Src/Material.cpp
    Test/Src/Matrix.cpp
    Test/Src/Pattern.cpp
    Test/Src/Plane.cpp
    Test/Src/Ray.cpp
    Test/Src/Shape.cpp
    Test/Src/Sphere.cpp
    Test/Src/TestHelpers.h
    Test/Src/TestHelpers.cpp
    Test/Src/Transformations.cpp
    Test/Src/Tuple.cpp
    Test/Src/World.cpp )

AddTarget( RayTracerTest EXECUTABLE
    FOLDER  02_Tests
    DEPS    RayTracer BeDDev
    SOURCES ${TEST_SOURCES}
    PRIVATE_DIRS "./Test/Src" "./Src" "../../External" )

# Sample projects #############################################################
#AddTarget( 00_MathIntrinsicsTests EXECUTABLE
#    FOLDER  03_Samples
#    DEPS    RayTracer
#    SOURCES Samples/Src/MathIntrinsicsTests.cpp )

AddTarget( Ch02_ProjectilTrajectory EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/ProjectilTrajectory.cpp )

AddTarget( Ch04_WallClock EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/WallClock.cpp )

AddTarget( Ch05_SphereSilhouette EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/SphereSilhouette.cpp )

AddTarget( Ch06_LitSphere EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/LitSphere.cpp )

AddTarget( Ch07_Scene EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/Scene.cpp )

AddTarget( Ch09_Planes EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/Plane.cpp )

AddTarget( Ch10_Patterns EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/Patterns.cpp )

AddTarget( Ch11_ReflectionAndRefraction EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/ReflectionAndRefraction.cpp )
