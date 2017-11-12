cmake_minimum_required(VERSION 2.8)
enable_testing()
project(nurbs)

# Disable Wextra for MSVC
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
else()
    set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")
endif()

include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(test)