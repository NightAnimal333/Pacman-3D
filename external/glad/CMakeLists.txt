# Add GLAD as a Library that we can link with

add_library(glad STATIC)
add_library(glad::glad ALIAS glad)

# Add source files
target_sources(
    glad
    # Public Sources
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/glad.h

    # Private Sources
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_LIST_DIR}/src/glad.c
)

# Set include directories
target_include_directories(
    glad
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/KHR
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
)
