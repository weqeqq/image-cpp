cmake_minimum_required(VERSION 3.23)

project(
  image LANGUAGES C CXX VERSION 0.1.0
)

find_package(file REQUIRED)

add_subdirectory(image-core)
add_subdirectory(image-codec)
add_subdirectory(image-io)
add_subdirectory(image-main)

