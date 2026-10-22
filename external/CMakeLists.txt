# Vendored dependencies: built together with the project.
# None of them requires a previous installation, network access or admin rights.
#
#   glfw 3.4    window, OpenGL context and event handling
#   glad 2.0.8  OpenGL 4.6 core loader (generated, no code generation needed)
#   glm 1.0.3   mathematics, header only
#   stb_image   image loading, single header

# --- GLFW ---------------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# On Linux we only build the X11 backend: Wayland would require additional
# system packages (libwayland-dev, wayland-protocols, libxkbcommon-dev) that
# students cannot install in the lab.
if(UNIX AND NOT APPLE)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(glfw)

# --- GLAD ---------------------------------------------------------------
add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

# --- GLM ----------------------------------------------------------------
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
# GLM_FORCE_RADIANS is not defined here on purpose: the project headers already
# define it, and defining it twice makes the compiler warn on every file.
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)

# --- stb_image ----------------------------------------------------------
add_library(stb_image STATIC stb/stb_image.c)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)
