cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# BeDDev library ##############################################################
set( LIB_SOURCES
    Include/Beddev/Beddev.h
	Include/Beddev/BeddevImpl.h
    Include/Beddev/ColoredOutput.h
	Include/Beddev/TestCase.h
	Include/Beddev/TestRunner.h
	Src/TestCase.cpp
	Src/TestRunner.cpp )

AddTarget( BeDDev LIBRARY EXPORT_HEADER
    FOLDER 01_Libs
    SOURCES ${LIB_SOURCES}
    INTERFACE_DIRS "./Include"
	PRIVATE_DIRS "./Include/Beddev" )

# BeDDev tests #############################################################
set( TEST_SOURCES
	Test/Src/Main.cpp
	Test/Src/Beddev.cpp )

AddTarget( BeDDevTests EXECUTABLE
    FOLDER  02_Tests
    DEPS    BeDDev
    SOURCES ${TEST_SOURCES}
    PRIVATE_DIRS "./Test/Src" )
