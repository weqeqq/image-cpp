
add_library(
  image_io)

add_library(
  image::io ALIAS image_io)

target_link_libraries(image_io
  PUBLIC
    file::file
    image::core

  PRIVATE
    image::codec
)
include(cmake/generate-export-header.cmake)

target_sources(image_io PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS include ${EXPORT_HEADER_BASE_DIR}
  FILES 
  include/image/io/decode.h
  include/image/io/encode.h

  ${EXPORT_HEADER_FILE_NAME}

  PRIVATE 
  source/decode.cc 
  source/encode.cc
)
