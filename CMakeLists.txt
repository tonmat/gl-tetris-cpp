cmake_minimum_required(VERSION 3.14)
project(gl_tetris_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

add_executable(gl_tetris_cpp
        libs/stb/stb_image.h
        src/main.cpp)

find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(gl_tetris_cpp PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
        ${GLFW_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR})

target_link_libraries(gl_tetris_cpp
        glfw3
        ${GLFW_LIBRARY_DIR}
        OpenGL::GL
        glm
        ${GLEW_LIBRARIES})

file(GLOB SHADERS "res/shaders/*.*")
file(GLOB TEXTURES "res/textures/*.png")
file(COPY ${SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)
file(COPY ${TEXTURES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures)