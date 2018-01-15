cmake_minimum_required(VERSION 2.8)
enable_testing()
project(nurbs)

# Disable Wextra for MSVC
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CXX_FLAGS "/std:c++17")
else()
    set(CXX_FLAGS "-std=c++1z -Wall -Wextra")
endif()

# merge cxx flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(test)
add_subdirectory(sample)