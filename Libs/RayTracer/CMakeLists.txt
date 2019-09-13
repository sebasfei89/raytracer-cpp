cmake_minimum_required( VERSION 3.14 )

# RayTracer library ###########################################################
AddTarget( RayTracer LIBRARY EXPORT_HEADER
    FOLDER 1.Libs
    SOURCES Src/*.cpp Include/RayTracer/*.h
    PUBLIC_DIRS "./Include"
    PRIVATE_DIRS "./Include/RayTracer" "./Src"
    #PUBLIC_DEFS "USE_SSE" "_ENABLE_EXTENDED_ALIGNED_STORAGE" stuff for MathIntrinsics.h
    PRIVATE_DEFS "RENDER_TASKS=12" )

# RayTracer tests #############################################################
AddTarget( RayTracerTest TEST
    FOLDER  2.Tests
    DEPS    RayTracer BeDDev
    SOURCES Test/Src/*.cpp Test/Src/*.h
    PRIVATE_DIRS "./Test/Src" )

# Sample projects #############################################################
AddTarget( SampleUtils LIBRARY EXPORT_HEADER
    FOLDER  3.Samples
    DEPS    RayTracer VulkApp
    SOURCES Samples/Src/SampleUtils.h Samples/Src/SampleUtils.cpp
    PUBLIC_DIRS "./Samples/Src"
    #PRIVATE_DEFS "SAVE_SCENE_AS_PPM"
)

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

AddTarget( Ch13_CylindersAndCones EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/CylindersAndCones.cpp )

AddTarget( Ch14_Groups EXECUTABLE
    FOLDER  3.Samples
    DEPS    RayTracer SampleUtils
    SOURCES Samples/Src/Groups.cpp
    PRIVATE_DIRS "../../ThirdParty/nlohmann-json-v3.7.0" )
