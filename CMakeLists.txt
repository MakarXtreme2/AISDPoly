cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ariths)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ARITHS_LIBRARY "${PROJECT_NAME}")
set(ARITHS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories("${ARITHS_INCLUDE}")

set(ANTLR_INCLUDE_DIR "/usr/local/include/antlr4-runtime")
include_directories("parser")
set(ANTLR_LIBRARY "/usr/local/lib64/libantlr4-runtime.so")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF)
set(HAVE_STD_REGEX 0)
set(HAVE_STEADY_CLOCK 0)
set(HAVE_POSIX_REGEX 0)
# BUILD
add_subdirectory(3rdparty)

enable_testing()

add_subdirectory(parser)
add_subdirectory(src)
add_subdirectory(test)

add_subdirectory(sample)
# REPORT
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message( STATUS "")
