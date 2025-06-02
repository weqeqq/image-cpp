
add_library(
  image_codec_jpeg)

add_library(
  image::codec::jpeg ALIAS image_codec_jpeg)

target_compile_features(
  image_codec_jpeg PUBLIC cxx_std_23)

include(cmake/target/jpeg-turbo.cmake)

target_link_libraries(image_codec_jpeg
  PUBLIC
    image::core
    target::file

  PRIVATE
    target::jpeg-turbo
)
include(cmake/generate-export-header.cmake)

target_sources(image_codec_jpeg PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS include ${EXPORT_HEADER_BASE_DIR}
  FILES 
  include/image/codec/jpeg/decode.h
  include/image/codec/jpeg/encode.h
  include/image/codec/jpeg.h

  ${EXPORT_HEADER_FILE_NAME}

  PRIVATE
  source/encode.cc 
  source/decode.cc
)
