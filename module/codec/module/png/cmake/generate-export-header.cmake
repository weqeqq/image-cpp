
include(GenerateExportHeader)

set(EXPORT_HEADER_BASE_DIR 
  ${CMAKE_CURRENT_BINARY_DIR}/export-header)

set(EXPORT_HEADER_FILE_NAME 
  ${EXPORT_HEADER_BASE_DIR}/image/codec/png/export.h)

generate_export_header(image_codec_png
  EXPORT_FILE_NAME 
  ${EXPORT_HEADER_FILE_NAME}
)
