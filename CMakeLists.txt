cmake_minimum_required (VERSION 3.14 FATAL_ERROR)
project(El_Pacmano)

# Add dependencies
add_subdirectory(external)
# Create .exe file (essentially)
# Create .exe file (essentially)
add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_library(irrKlang SHARED IMPORTED)
if (WIN32)
  set_target_properties(irrKlang PROPERTIES LINKER_LANGUAGE CXX
                                  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/bin/winx64-visualStudio/irrKlang.dll"
                                  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/bin/winx64-visualStudio/ikpMP3.dll"
                                  IMPORTED_IMPLIB   "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/lib/Winx64-visualStudio/irrKlang.lib"
                                  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/include")
endif (WIN32)
if (UNIX)
  set_target_properties(irrKlang PROPERTIES LINKER_LANGUAGE CXX
                                  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/bin/linux-gcc/irrKlang.so"
                                  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/bin/linux-gcc/ikpMP3.so"
                                  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/include")
endif (UNIX)


set_target_properties(irrKlang PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(irrKlang PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Add additional source files here
target_sources(
    ${PROJECT_NAME}
    PRIVATE
	# Group made files
    ${CMAKE_CURRENT_LIST_DIR}/src/camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/entity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/loader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/settings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/vertex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/window.cpp


    ${CMAKE_CURRENT_LIST_DIR}/include/camera.h
    ${CMAKE_CURRENT_LIST_DIR}/include/entity.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gameState.h
    ${CMAKE_CURRENT_LIST_DIR}/include/loader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/mesh.h
    ${CMAKE_CURRENT_LIST_DIR}/include/settings.h
    ${CMAKE_CURRENT_LIST_DIR}/include/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/vertex.h
    ${CMAKE_CURRENT_LIST_DIR}/include/window.h


    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imconfig.h
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui.h
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_impl_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui/imgui_widgets.cpp

)

# Set include directories
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/external/imgui
    ${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/bin/winx64-visualStudio
    ${CMAKE_CURRENT_LIST_DIR}/external/irrKlang/include/
)

# Link with external libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glad::glad
    gfx::gfx
    glm
    glfw
    stbimage::stbimage
    tinyobj

    irrKlang
)

# Enable C++ 17
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_17
)

# Copy resources to binary directory
add_custom_target(
    copy_shaders ALL
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMAND ${CMAKE_COMMAND} -E echo "Copied resource directory to binary directory ${CMAKE_CURRENT_BINARY_DIR}"
)
