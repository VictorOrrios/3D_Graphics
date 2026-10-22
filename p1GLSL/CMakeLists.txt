project(p1GLSL VERSION 1.0.0 LANGUAGES CXX)

set(p1GLSL_SOURCES
    main.cpp
)

add_executable(p1GLSL ${p1GLSL_SOURCES})

target_include_directories(p1GLSL PUBLIC ${IGL_INCLUDE})

# IGL already propagates GLFW, GLAD, GLM and stb_image
target_link_libraries(p1GLSL IGL)

# Define absolute path to shaders folder
target_compile_definitions(p1GLSL PRIVATE SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders_P1")

# Define absolute path to textures folder
target_compile_definitions(p1GLSL PRIVATE TEXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../img")
