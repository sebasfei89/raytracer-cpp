cmake_minimum_required( VERSION 3.14 )

include(GenerateExportHeader)

function(AddBinary NAME)
    set( options EXECUTABLE LIBRARY EXPORT_HEADER )
    set( oneValueArgs FOLDER )
    set( multiValueArgs SOURCES DEPS PUBLIC_DIRS PRIVATE_DIRS )
    cmake_parse_arguments( ADDBINARY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if (ADDBINARY_EXECUTABLE)
        add_executable( ${NAME} )
    elseif (ADDBINARY_LIBRARY)
        add_library( ${NAME} )
    else()
        message( FATAL_ERROR "AddBinary expects on of EXECUTABLE or LIBRARY to be set" )
    endif()

    if (ADDBINARY_EXPORT_HEADER)
        generate_export_header( ${NAME} )
    endif()

    set_property( TARGET ${NAME} PROPERTY CXX_STANDARD 17 )
    set_property( TARGET ${NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${NAME}>" )

    if (DEFINED ADDBINARY_FOLDER)
        set_property(
            TARGET ${NAME}
            PROPERTY FOLDER ${ADDBINARY_FOLDER} )
    endif()

    target_sources( ${NAME} PRIVATE ${ADDBINARY_SOURCES} )
    source_group( TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ADDBINARY_SOURCES} )

    target_include_directories( ${NAME}
        PUBLIC ${ADDBINARY_PUBLIC_DIRS}
        PRIVATE ${ADDBINARY_PRIVATE_DIRS} )

    if (DEFINED ADDBINARY_DEPS)
        foreach(DEP ${ADDBINARY_DEPS})
            target_link_libraries( ${NAME} PRIVATE ${DEP} )
        endforeach()
    endif()
endfunction()
