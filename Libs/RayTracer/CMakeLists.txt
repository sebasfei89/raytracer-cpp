cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# RayTracer library ###########################################################
set( RT_SOURCES
    Include/Canvas.h
    Include/Color.h
    Include/Intersection.h
    Include/Matrix.h
    Include/Ray.h
    Include/Transformations.h
    Include/Tuple.h
    Include/Util.h
    Src/Canvas.cpp
    Src/Intersection.cpp
    Src/Ray.cpp
    Src/Transformations.cpp
    Src/Tuple.cpp )

AddBinary( RayTracer LIBRARY EXPORT_HEADER
    FOLDER 01_Libs
    SOURCES ${RT_SOURCES}
    PUBLIC_DIRS "./Include" "${CMAKE_BINARY_DIR}/Libs/RayTracer"
    PRIVATE_DIRS "./Src" )

# RayTracer tests #############################################################
set( TEST_SOURCES
    Test/Src/Canvas.cpp
    Test/Src/Color.cpp
    Test/Src/Intersection.cpp
    Test/Src/Main.cpp
    Test/Src/Matrix.cpp
    Test/Src/Ray.cpp
    Test/Src/Transformations.cpp
    Test/Src/Tuple.cpp )

AddBinary( RayTracerTest EXECUTABLE
    FOLDER  02_Tests
    DEPS    RayTracer
    SOURCES ${TEST_SOURCES}
    PRIVATE_DIRS "./Test/Src" "./Src" "../../External" )

# Sample projects #############################################################
AddBinary( ProjectilTrajectory EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/ProjectilTrajectory.cpp )

AddBinary( WallClock EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/WallClock.cpp )

AddBinary( SphereSilhouette EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/SphereSilhouette.cpp )
