cmake_minimum_required(VERSION 3.23)

project(RFS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)


# Create your game executable target as usual
add_executable(RFS
    main.cpp
    UV/Texturing.cpp
    Bresenhem/Bresenhem.cpp
    Math/Math.cpp
    Vertex/VertexShading.cpp
    Rasterizer/Rasterizer.cpp
)

# Link to the actual SDL3 library.
target_link_libraries(RFS PRIVATE SDL3::SDL3)
target_include_directories(RFS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})