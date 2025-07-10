
add_library(image INTERFACE)
add_library(image::image ALIAS image)
target_link_libraries(image
  INTERFACE
    image::core
    image::processing
)
target_sources(image
    PUBLIC
        FILE_SET HEADERS BASE_DIRS
            headers
        FILES
            headers/image/image.h
)
