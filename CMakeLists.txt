cmake_minimum_required(VERSION 3.1)

# ------------------------------------------
# Project settings
# ------------------------------------------
project(MOCABE_NURBS)

set(MOCABE_NURBS_DIR ${CMAKE_CURRENT_LIST_DIR})

# ------------------------------------------
# Add library
# ------------------------------------------
add_library(mocabe_nurbs INTERFACE)


# ------------------------------------------
# Library options
# ------------------------------------------
set_target_properties(mocabe_nurbs PROPERTIES
  INTERFACE_COMPILE_FEATURES cxx_std_17
)

# ------------------------------------------
# CTest
# ------------------------------------------
include(CTest)
enable_testing()

# ------------------------------------------
# Options
# ------------------------------------------
option(MOCABE_NURBS_BUILD_TEST "Build test for nurbs library" ON)
option(MOCABE_NURBS_BUILD_EXAMPLE "Build example for nurbs library" ON) 

# ------------------------------------------
# Initialize submodule
# ------------------------------------------
# include(external/external.cmake)

# ------------------------------------------
# set include directory
# ------------------------------------------
target_include_directories(mocabe_nurbs INTERFACE ${MOCABE_NURBS_DIR}/include)

# build test
if(MOCABE_NURBS_BUILD_TEST)
  add_subdirectory(test)
endif()

# build sample
if(MOCABE_NURBS_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()