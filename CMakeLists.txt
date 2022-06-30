cmake_minimum_required(VERSION 3.22.1)

project(
    Canvas
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(app)