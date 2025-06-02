
add_library(
  image_codec INTERFACE)
add_library(
  image::codec ALIAS image_codec)

add_subdirectory(png)
add_subdirectory(jpeg)
add_subdirectory(webp)

target_link_libraries(
  image_codec
  INTERFACE
  image::codec::png
  image::codec::jpeg
  image::codec::webp
)


