
add_library(image-codec-jpeg)
add_library(image::codec::jpeg ALIAS image-codec-jpeg)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LIB_DIR lib)
else()
    set(LIB_DIR lib64)
endif()
Dependency(
    libjpeg-turbo/libjpeg-turbo
    36ac5b847047b27b90b459f5d44154773880196f
    DEFINE
        ENABLE_STATIC ON
        ENABLE_SHARED OFF
    EXPECTED
        ${LIB_DIR}/libjpeg.a
        ${LIB_DIR}/libturbojpeg.a
)
target_link_libraries(image-codec-jpeg
  PUBLIC
    image::core
    image::external::file-cpp

  PRIVATE
    image::external::libjpeg-turbo
)
include(GenerateExportHeader)
generate_export_header(
    image-codec-jpeg
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/headers/image/codec/jpeg/export.h
)
target_sources(
    image-codec-jpeg
    PUBLIC
        FILE_SET HEADERS BASE_DIRS
            headers
            ${CMAKE_CURRENT_BINARY_DIR}/headers
        FILES
            headers/image/codec/jpeg/decode.h
            headers/image/codec/jpeg/encode.h
            headers/image/codec/jpeg.h
            ${CMAKE_CURRENT_BINARY_DIR}/headers/image/codec/jpeg/export.h
    PRIVATE
        sources/encode.cc
        sources/decode.cc
)
