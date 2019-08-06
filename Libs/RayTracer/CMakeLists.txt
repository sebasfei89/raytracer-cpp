cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# RayTracer library ###########################################################
set( RT_SOURCES
    Include/RayTracer/Camera.h
    Include/RayTracer/Canvas.h
    Include/RayTracer/Color.h
    Include/RayTracer/Cube.h
    Include/RayTracer/Intersection.h
    Include/RayTracer/Lighting.h
    Include/RayTracer/Material.h
    Include/RayTracer/Matrix.h
    Include/RayTracer/Pattern.h
    Include/RayTracer/Plane.h
    Include/RayTracer/Ray.h
    Include/RayTracer/Shape.h
    Include/RayTracer/SimplexNoise.h
    Include/RayTracer/Sphere.h
    Include/RayTracer/Transformations.h
    Include/RayTracer/Tuple.h
    Include/RayTracer/Types.h
    Include/RayTracer/Util.h
    Include/RayTracer/World.h
    Src/Camera.cpp
    Src/Canvas.cpp
    Src/Cube.cpp
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
    FOLDER 1.Libs
    SOURCES ${RT_SOURCES}
    PUBLIC_DIRS "./Include"
    PRIVATE_DIRS "./Include/RayTracer" "./Src"
    #PUBLIC_DEFS "USE_SSE" "_ENABLE_EXTENDED_ALIGNED_STORAGE" stuff for MathIntrinsics.h
    PRIVATE_DEFS "RENDER_TASKS=12" )

# RayTracer tests #############################################################
set( TEST_SOURCES
    Test/Src/Canvas.cpp
    Test/Src/Camera.cpp
    Test/Src/Color.cpp
    Test/Src/Cube.cpp
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

AddTarget( RayTracerTest TEST
    FOLDER  2.Tests
    DEPS    RayTracer BeDDev
    SOURCES ${TEST_SOURCES}
    PRIVATE_DIRS "./Test/Src" )

# Sample projects #############################################################
AddTarget( SampleUtils LIBRARY EXPORT_HEADER
    FOLDER  3.Samples
    DEPS    RayTracer
    SOURCES Samples/Src/SampleUtils.h Samples/Src/SampleUtils.cpp
    PUBLIC_DIRS "./Samples/Src" )

#AddTarget( 00_MathIntrinsicsTests EXECUTABLE
#    FOLDER  3.Samples
#    DEPS    RayTracer
#    SOURCES Samples/Src/MathIntrinsicsTests.cpp )

AddTarget( Ch02_ProjectilTrajectory EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/ProjectilTrajectory.cpp )

AddTarget( Ch04_WallClock EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/WallClock.cpp )

AddTarget( Ch05_SphereSilhouette EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/SphereSilhouette.cpp )

AddTarget( Ch06_LitSphere EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/LitSphere.cpp )

AddTarget( Ch07_Scene EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/Scene.cpp )

AddTarget( Ch09_Planes EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/Plane.cpp )

AddTarget( Ch10_Patterns EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/Patterns.cpp )

AddTarget( Ch11a_ReflectionAndRefraction EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/ReflectionAndRefraction.cpp )

AddTarget( Ch11b_FresnelEffect EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/FresnelEffect.cpp )

AddTarget( Ch12_Cubes EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/Cubes.cpp )
