cmake_minimum_required( VERSION 3.14 )

# BeDDev library ###############################################################
AddTarget( BeDDev LIBRARY EXPORT_HEADER
    FOLDER 1.Libs
    SOURCES Include/Beddev/*.h Src/*.cpp
    INTERFACE_DIRS "./Include"
    PRIVATE_DIRS "./Include/Beddev" )

# BeDDev tests #################################################################
AddTarget( BeDDevTests TEST
    FOLDER  2.Tests
    DEPS    BeDDev
    SOURCES Test/Src/*.cpp
    PRIVATE_DIRS "./Test/Src" )
