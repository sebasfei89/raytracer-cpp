cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# RayTracer library ###########################################################
AddBinary( RayTracer LIBRARY EXPORT_HEADER
    FOLDER 01_Libs
    SOURCES
        Include/Canvas.h
        Include/Color.h
        Include/Tuple.h
        Src/Canvas.cpp
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
    FOLDER  02_Tests
    DEPS    RayTracer
    SOURCES
        Test/Src/Canvas.cpp
        Test/Src/Color.cpp
        Test/Src/Main.cpp
        Test/Src/Tuple.cpp
    PRIVATE_DIRS
        "./Test/Src"
        "./Src"
        "../../External" )

# Sample projects #############################################################
AddBinary( RayTracerSampleApp EXECUTABLE
    FOLDER  03_Samples
    DEPS    RayTracer
    SOURCES Samples/Src/Main.cpp)
