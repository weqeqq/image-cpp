cmake_minimum_required(VERSION 3.23)
project(image C CXX)

set(PROJECT_VERSION "0.2.0")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

option(IMAGE_DEPENDENCY_FILE_IMPORTED "" OFF)
option(IMAGE_DEPENDENCY_FILE_FETCH    "" OFF)

include(FetchContent)
FetchContent_Declare(
  dependency-cmake
  GIT_REPOSITORY https://github.com/weqeqq/dependency-cmake
  GIT_TAG        e982bc12360ac70cbef756c90c23cddb50c9d501
)
FetchContent_MakeAvailable(dependency-cmake)
if(NOT IMAGE_DEPENDENCY_FILE_IMPORTED)
    if(IMAGE_DEPENDENCY_FILE_FETCH)
        Dependency(
            weqeqq/file-cpp
            d7eda176f32ec5b45eac172e2cddb08d562d3ebb
        )
    else()
        find_package(file REQUIRED)
        add_library(image::external::file-cpp ALIAS file::file)
    endif()
endif()
if(IMAGE_DEPENDENCY_FILE_IMPORTED)
    add_library(image::external::file-cpp ALIAS file)
endif()
add_subdirectory(module/core)
add_subdirectory(module/codec)
add_subdirectory(module/io)
add_subdirectory(module/processing)
add_subdirectory(lib)
