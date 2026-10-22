project( IGL VERSION 1.0.0 LANGUAGES CXX )

set( IGL_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/IGL/IGlib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/IGL/BOX.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/IGL/PLANE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/IGL/auxiliar.h
)

set( IGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IGlib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/OGLObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/auxiliar.cpp
)

set(IGL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE INTERNAL "" )

add_library( IGL  ${IGL_HEADERS} ${IGL_SOURCES})

target_include_directories(IGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries( IGL PUBLIC glfw glad glm::glm stb_image )
