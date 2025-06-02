
add_library(
  image_core INTERFACE)

add_library(
  image::core ALIAS image_core)

target_compile_features(
  image_core INTERFACE cxx_std_23)

target_sources(image_core PUBLIC
  FILE_SET  HEADERS 
  BASE_DIRS include
  FILES 
  include/image/core/error.h
)


