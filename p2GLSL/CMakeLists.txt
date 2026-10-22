project(p2GLSL VERSION 1.0.0 LANGUAGES CXX)

set(p2GLSL_SOURCES
    main.cpp
)

add_executable(p2GLSL ${p2GLSL_SOURCES})

target_include_directories(p2GLSL PUBLIC ${IGL_INCLUDE})

# IGL already propagates GLFW, GLAD, GLM and stb_image
target_link_libraries(p2GLSL IGL)

# Define absolute path to shaders folder
target_compile_definitions(p2GLSL PRIVATE SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders_P2")

# Define absolute path to textures folder
target_compile_definitions(p2GLSL PRIVATE TEXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../img")
