cmake_minimum_required(VERSION 3.23)

project(image LANGUAGES C CXX VERSION 0.1.0)

add_library(image INTERFACE)
add_library(image::image ALIAS image)

target_sources(image INTERFACE
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
  include/image/element.h
)
