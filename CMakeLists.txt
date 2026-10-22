cmake_minimum_required(VERSION 3.16)

project( g3d-practicas VERSION 0.1.0 LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install/)

# All dependencies (GLFW, GLAD, GLM, stb_image) live in external/ and are built
# together with the project: nothing has to be installed on the machine.
add_subdirectory( external )

add_subdirectory( IGL )

add_subdirectory(p1GLSL)
add_subdirectory(p2GLSL)
add_subdirectory(p3OGL)
add_subdirectory(p4OGL)
