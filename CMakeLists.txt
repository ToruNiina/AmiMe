cmake_minimum_required(VERSION 2.8)
project(amime)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
include_directories(${PROJECT_SOURCE_DIR})

add_definitions("-std=c++11")
set(CMAKE_CXX_COMPILER clang++-3.6)
add_subdirectory(src)
