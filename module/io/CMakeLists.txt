
add_library(image-io)
add_library(image::io ALIAS image-io)

target_link_libraries(image-io
  PUBLIC
    image::external::file-cpp
    image::core
  PRIVATE
    image::codec
)
include(GenerateExportHeader)
generate_export_header(
    image-io
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/headers/image/io/export.h
)
target_sources(
    image-io
    PUBLIC
        FILE_SET HEADERS BASE_DIRS
            headers
            ${CMAKE_CURRENT_BINARY_DIR}/headers
        FILES
            headers/image/io/decode.h
            headers/image/io/encode.h
            ${CMAKE_CURRENT_BINARY_DIR}/headers/image/io/export.h
    PRIVATE
        sources/decode.cc
        sources/encode.cc
)
if(IMAGE_FORMAT_JPEG)
target_compile_definitions(image-io IMAGE_FORMAT_JPEG)
endif()
