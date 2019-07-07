cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# RayTracer library ###########################################################
set( RT_SOURCES
    Include/Canvas.h
    Include/Color.h
    Include/Matrix.h
    Include/Tuple.h
    Src/Canvas.cpp
    Src/Tuple.cpp
    Src/Util.h)

AddBinary( RayTracer LIBRARY EXPORT_HEADER
    FOLDER 01_Libs
    SOURCES ${RT_SOURCES}
    PUBLIC_DIRS "./Include" "${CMAKE_BINARY_DIR}/Libs/RayTracer"
    PRIVATE_DIRS "./Src" "../../External" )

# RayTracer tests #############################################################
set( TEST_SOURCES
    Test/Src/Canvas.cpp
    Test/Src/Color.cpp
    Test/Src/Main.cpp
    Test/Src/Matrix.cpp
    Test/Src/Tuple.cpp)

AddBinary( RayTracerTest EXECUTABLE
    FOLDER  02_Tests
    DEPS    RayTracer
    SOURCES ${TEST_SOURCES}
    PRIVATE_DIRS "./Test/Src" "./Src" "../../External" )

# Sample projects #############################################################
AddBinary( RayTracerSampleApp EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/Main.cpp )
