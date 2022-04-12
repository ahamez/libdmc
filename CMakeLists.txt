cmake_minimum_required (VERSION 2.8.9)

#--------------------------------------------------------------------------------------------------#

project (libdmc)

#--------------------------------------------------------------------------------------------------#

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX14)
if(NOT COMPILER_SUPPORTS_CXX14)
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support.")
endif ()

#--------------------------------------------------------------------------------------------------#

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

find_package (Threads)

add_definitions("-Wall")
add_definitions("-Wextra")
add_definitions("-std=c++1y")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
  endif ()
endif ()

#--------------------------------------------------------------------------------------------------#

include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC "${PROJECT_SOURCE_DIR}/src/*.cc" "${PROJECT_SOURCE_DIR}/src/*.hh")
add_library(libdmc STATIC ${SRC})

#--------------------------------------------------------------------------------------------------#
