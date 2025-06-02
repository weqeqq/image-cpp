
add_library(
  image_main INTERFACE
)
add_library(
  image::main ALIAS image_main
)
target_link_libraries(image_main
  INTERFACE
    image::core
    image::io
)
target_sources(image_main PUBLIC
  FILE_SET  HEADERS 
  BASE_DIRS include
  FILES 
  include/image/image.h
)
