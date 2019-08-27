cmake_minimum_required( VERSION 3.14 )

# VulkApp library ##############################################################
AddTarget( VulkApp LIBRARY EXPORT_HEADER
    FOLDER       1.Libs
    SOURCES      Include/VulkApp/*.h Src/*.cpp Resources/Shaders/*.glsl
    PUBLIC_DIRS  ./Include
    PRIVATE_DIRS ./Include/VulkApp ./Src )

# GLFW3
find_package( glfw3 CONFIG REQUIRED )
target_link_libraries( VulkApp PUBLIC glfw )

# GLM
find_package( glm REQUIRED )
target_link_libraries( VulkApp PRIVATE glm )

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( VulkApp PUBLIC Vulkan::Vulkan )

# ImGUI
find_package( imgui CONFIG REQUIRED )
target_link_libraries( VulkApp PRIVATE imgui::imgui )

add_custom_command(TARGET VulkApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:VulkApp>/Resources/Shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/Resources/Shaders/" "$<TARGET_FILE_DIR:VulkApp>/Resources/Shaders/"
    COMMENT "Copying resources to output directory..." )

CompileShaders( SPIRV_BINARY_FILES "Resources/Shaders/*.glsl" "${PROJECT_BINARY_DIR}/Resources/Shaders" )
add_custom_target( VulkAppShaders DEPENDS ${SPIRV_BINARY_FILES} )
set_property( TARGET VulkAppShaders PROPERTY FOLDER 4.Utilities )
add_dependencies( VulkApp VulkAppShaders )

# VulkApp tests ################################################################
AddTarget( VulkAppTest TEST
    FOLDER       2.Tests
    DEPS         VulkApp BeDDev
    SOURCES      Test/Src/*.cpp )
