
add_library(
  image_codec_png)

add_library(
  image::codec::png ALIAS image_codec_png)

target_compile_features(
  image_codec_png PUBLIC cxx_std_23)

include(cmake/target/wuffs.cmake)
include(cmake/target/fpng.cmake)

target_link_libraries(image_codec_png
  PUBLIC
    image::core
    file::file

  PRIVATE
    target::wuffs 
    target::fpng
)
include(cmake/generate-export-header.cmake)

target_sources(image_codec_png PUBLIC 
  FILE_SET  HEADERS 
  BASE_DIRS include ${EXPORT_HEADER_BASE_DIR}
  FILES 
  include/image/codec/png/decode.h
  include/image/codec/png/encode.h
  include/image/codec/png.h

  ${EXPORT_HEADER_FILE_NAME}

  PRIVATE
  source/decode.cc
  source/encode.cc
)
