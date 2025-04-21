cmake_minimum_required(VERSION 3.23)

project(image LANGUAGES C CXX VERSION 0.1.0)

include(cmake/fpng-target.cmake)
include(cmake/jpeg-turbo-target.cmake)
include(cmake/wuffs-target.cmake)

add_library(image)
add_library(image::image ALIAS image)

target_sources(image PRIVATE 
  source/decode_impl/png.cc
  source/decode_impl/jpeg.cc
  source/encode_impl/png.cc
  source/capi/buffer.cc
  source/capi/decode.cc
  source/capi/encode.cc
)

target_sources(image PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS include
  FILES 
  include/image/convertor/bitmap.h
  include/image/convertor/channel_array.h
  include/image/convertor/color.h  
  include/image/convertor/data.h  
  include/image/convertor/depth.h  
  include/image/convertor/endianness.h  
  include/image/convertor/sequence.h  
  include/image/core/color.h 
  include/image/core/depth.h 
  include/image/core/endianness.h 
  include/image/core/error.h 
  include/image/core/format.h 
  include/image/processing/blend.h
  include/image/buffer.h 
  include/image/decode.h 
  include/image/element.h 
  include/image/encode.h)

target_link_libraries(image PRIVATE
  fpng
  jpeg-turbo 
  wuffs)

target_compile_options(image PRIVATE -O3 -Wall -Wpedantic)

install(TARGETS image EXPORT image-targets FILE_SET HEADERS)
install(EXPORT image-targets NAMESPACE image:: DESTINATION lib/cmake/image)

include(CMakePackageConfigHelpers)

set(IMAGE_INCLUDE_DIR include)
set(IMAGE_LIBRARY_DIR     lib)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/image-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/image-config.cmake 
  INSTALL_DESTINATION lib/cmake/image PATH_VARS IMAGE_LIBRARY_DIR IMAGE_INCLUDE_DIR)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/image-version.cmake COMPATIBILITY SameMajorVersion)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/image-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/image-version.cmake
  DESTINATION lib/cmake/image
)
