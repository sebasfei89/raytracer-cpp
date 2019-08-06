cmake_minimum_required( VERSION 3.14 )

include(GenerateExportHeader)

function(AddTarget NAME)
    set( options EXECUTABLE LIBRARY TEST INTERFACE EXPORT_HEADER )
    set( oneValueArgs FOLDER )
    set( multiValueArgs SOURCES DEPS INTERFACE_DIRS PUBLIC_DIRS PRIVATE_DIRS PUBLIC_DEFS PRIVATE_DEFS )
    cmake_parse_arguments( ADDTARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if (ADDTARGET_EXECUTABLE OR ADDTARGET_TEST)
        add_executable( ${NAME} )
    elseif (ADDTARGET_LIBRARY)
        add_library( ${NAME} )
    elseif (ADDTARGET_INTERFACE)
        add_library( ${NAME} INTERFACE )
    else()
        message( FATAL_ERROR "AddTarget expects one of EXECUTABLE, LIBRARY or INTERFACE options set" )
    endif()

    if (ADDTARGET_TEST)
        add_test( NAME ${NAME}_TEST COMMAND ${NAME} )
    endif()

    if (ADDTARGET_EXPORT_HEADER)
        generate_export_header( ${NAME} )
    endif()

    if (ADDTARGET_EXECUTABLE OR ADDTARGET_LIBRARY OR ADDTARGET_TEST)

        target_compile_features( ${NAME} PUBLIC cxx_std_17 )

        set_property( TARGET ${NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${NAME}>" )
        if (DEFINED ADDTARGET_FOLDER)
            set_property( TARGET ${NAME} PROPERTY FOLDER ${ADDTARGET_FOLDER} )
        endif()

        target_sources( ${NAME} PRIVATE ${ADDTARGET_SOURCES} )
        list(APPEND ADDTARGET_PUBLIC_DIRS "${CMAKE_BINARY_DIR}/Libs/${NAME}")
        target_include_directories( ${NAME} INTERFACE ${ADDTARGET_INTERFACE_DIRS} PUBLIC ${ADDTARGET_PUBLIC_DIRS} PRIVATE ${ADDTARGET_PRIVATE_DIRS} )

    elseif (ADDTARGET_INTERFACE)

        target_compile_features( ${NAME} INTERFACE cxx_std_17 )

        target_sources( ${NAME} INTERFACE ${ADDTARGET_SOURCES} )
        target_include_directories( ${NAME} INTERFACE ${ADDTARGET_INTERFACE_DIRS} )

    endif()

    source_group( TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ADDTARGET_SOURCES} )

    if (DEFINED ADDTARGET_DEPS)
        foreach(DEP ${ADDTARGET_DEPS})
            target_link_libraries( ${NAME} PRIVATE ${DEP} )
        endforeach()
    endif()

	if (DEFINED ADDTARGET_PUBLIC_DEFS)
		foreach(DEF ${ADDTARGET_PUBLIC_DEFS})
			target_compile_definitions( ${NAME} PUBLIC ${DEF} )
        endforeach()
	endif()

	if (DEFINED ADDTARGET_PRIVATE_DEFS)
		foreach(PRIV_DEF ${ADDTARGET_PRIVATE_DEFS})
			target_compile_definitions( ${NAME} PRIVATE ${PRIV_DEF} )
        endforeach()
	endif()
endfunction()
