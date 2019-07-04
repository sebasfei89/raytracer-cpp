cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# RayTracer library ###########################################################
AddBinary( RayTracer LIBRARY EXPORT_HEADER
    FOLDER Libs
    SOURCES
        Include/Tuple.h
        Src/Tuple.cpp
        Src/Util.h
    PUBLIC_DIRS
        "./Include"
        "${CMAKE_BINARY_DIR}/Libs/RayTracer"
    PRIVATE_DIRS
        "./Src"
        "../../External" )

# RayTracer tests #############################################################
AddBinary( RayTracerTest EXECUTABLE
    FOLDER  Tests
    DEPS    RayTracer
    SOURCES
        Test/Src/Main.cpp
        Test/Src/Tuple.cpp
    PRIVATE_DIRS
        "./Test/Src"
        "./Src"
        "../../External" )

# Sample projects #############################################################
AddBinary( Ch1SampleApp EXECUTABLE
    FOLDER  Samples
    DEPS    RayTracer
    SOURCES Samples/Chapter1/Src/Main.cpp)
