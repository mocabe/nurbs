cmake_minimum_required(VERSION 3.1)

# ------------------------------------------
# Project settings
# ------------------------------------------
project(mocabe_nurbs)

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
if(MOCABE_NURBS_BUILD_TEST) 
  include(CTest)
  enable_testing()
endif()

# ------------------------------------------
# Catch2
# ------------------------------------------
if(MOCABE_NURBS_BUILD_TEST)

  # init Catch2 submodule
  find_package(Git REQUIRED)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init external/Catch2
    WORKING_DIRECTORY ${MOCABE_NURBS_DIR})
  add_subdirectory(external/Catch2)

  add_library(test-catch test/catch.cpp)
  target_link_libraries(test-catch PRIVATE Catch2::Catch2)
endif()

# ------------------------------------------
# Options
# ------------------------------------------
option(MOCABE_NURBS_BUILD_TEST "Build test for nurbs library" ON)
option(MOCABE_NURBS_BUILD_EXAMPLE "Build example for nurbs library" ON) 

# ------------------------------------------
# set include directory
# ------------------------------------------
target_include_directories(mocabe_nurbs INTERFACE ${MOCABE_NURBS_DIR}/include)
add_library(mocabe::nurbs ALIAS mocabe_nurbs)

# ------------------------------------------
# test script
# ------------------------------------------
function(MOCABE_NURBS_TEST NAME) 
  set(TARGET test-${NAME})
  add_test(NAME ${TARGET} COMMAND ${TARGET})
  add_executable(${TARGET} ${NAME}.cpp)
  target_link_libraries(${TARGET} PRIVATE mocabe::nurbs)
  target_link_libraries(${TARGET} PRIVATE Catch2::Catch2)
  target_link_libraries(${TARGET} PRIVATE test-catch)
endfunction()

# build tests
if(MOCABE_NURBS_BUILD_TEST)
  add_subdirectory(test)
endif()

# build examples
if(MOCABE_NURBS_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()