
add_library(image-processing INTERFACE)
add_library(image::processing ALIAS image-processing)

target_link_libraries(
    image-processing
    INTERFACE
        image::core
)
target_sources(
    image-processing
    INTERFACE
        FILE_SET HEADERS BASE_DIRS
            headers
        FILES
            headers/image/processing/blend.h
            headers/image/processing/blend.h
)
