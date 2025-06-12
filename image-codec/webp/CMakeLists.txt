
add_library(
  image_codec_webp)

add_library(
  image::codec::webp ALIAS image_codec_webp)

target_compile_features(
  image_codec_webp PUBLIC cxx_std_23)

include(cmake/target/webp.cmake)

target_link_libraries(image_codec_webp
  PUBLIC
    image::core
    file::file

  PRIVATE
    target::webp
)
include(cmake/generate-export-header.cmake)

target_sources(image_codec_webp PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS include ${EXPORT_HEADER_BASE_DIR}
  FILES 
  include/image/codec/webp/color.h
  include/image/codec/webp/decode.h
  include/image/codec/webp/encode.h
  include/image/codec/webp/error.h
  include/image/codec/webp.h

  ${EXPORT_HEADER_FILE_NAME}

  PRIVATE
  source/decode.cc
  source/encode.cc
)
