cmake_minimum_required(VERSION 2.8)
enable_testing()
project(nurbs)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(test)