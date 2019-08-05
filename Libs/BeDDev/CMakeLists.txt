cmake_minimum_required( VERSION 3.14 )

include(../../BuildSystem/Utils.cmake)

# BeDDev library ##############################################################
set( LIB_SOURCES
    Include/Beddev/Beddev.h
	Include/Beddev/BeddevImpl.h
    Include/Beddev/ColoredOutput.h
	Include/Beddev/Expression.h
	Include/Beddev/ExpressionParser.h
	Include/Beddev/TestCase.h
	Include/Beddev/TestRunner.h
	Src/TestRunner.cpp )

AddTarget( BeDDev LIBRARY EXPORT_HEADER
    FOLDER 1.Libs
    SOURCES ${LIB_SOURCES}
    INTERFACE_DIRS "./Include"
	PRIVATE_DIRS "./Include/Beddev" )

# BeDDev tests #############################################################
set( TEST_SOURCES
    Test/Src/Beddev.cpp
    Test/Src/ExpressionParser.cpp
    Test/Src/Main.cpp
    Test/Src/ParametrizedTestCase.cpp
    Test/Src/TestCase.cpp
    Test/Src/TestRunner.cpp )

AddTarget( BeDDevTests EXECUTABLE
    FOLDER  2.Tests
    DEPS    BeDDev
    SOURCES ${TEST_SOURCES}
    PRIVATE_DIRS "./Test/Src" )
