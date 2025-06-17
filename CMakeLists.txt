cmake_minimum_required(VERSION 3.23)
project(image C CXX)

set(PROJECT_VERSION "0.1.0")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

# TODO
#
# option(IMAGE_FETCH_PUBLIC "Fetch public dependencies" OFF)
# if(IMAGE_FETCH_PUBLIC) 
#   set(IMAGE_FETCH_FILE ON)
# else()
#   option(IMAGE_FETCH_FILE "Fetch file dependency" OFF)
# endif()
#

find_package(file REQUIRED)

add_subdirectory(module/core)
add_subdirectory(module/codec)
add_subdirectory(module/io)

add_subdirectory(lib)

