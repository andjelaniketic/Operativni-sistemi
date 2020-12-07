cmake_minimum_required(VERSION 3.17)
project(hello_triangle)

set(CMAKE_CXX_STANDARD 14)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(glad/)
add_executable(hello_triangle main.cpp)

target_link_libraries(hello_triangle glad glfw OpenGL::GL pthread X11 Xrandr Xi dl)