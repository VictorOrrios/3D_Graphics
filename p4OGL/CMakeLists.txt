project(p4OGL VERSION 1.0.0 LANGUAGES CXX)

set(p4OGL_SOURCES
    main.cpp
)

add_executable(p4OGL ${p4OGL_SOURCES})

target_include_directories(p4OGL PUBLIC ${IGL_INCLUDE})

# IGL already propagates GLFW, GLAD, GLM and stb_image
target_link_libraries(p4OGL IGL)

# Define absolute path to shaders folder
target_compile_definitions(p4OGL PRIVATE SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders_P4")

# Define absolute path to textures folder
target_compile_definitions(p4OGL PRIVATE TEXTURES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../img")
