
add_library(
  image_codec INTERFACE)
add_library(
  image::codec ALIAS image_codec)

add_subdirectory(module/png)
add_subdirectory(module/jpeg)
add_subdirectory(module/webp)

target_link_libraries(
  image_codec
  INTERFACE
  image::codec::png
  image::codec::jpeg
  image::codec::webp
)


