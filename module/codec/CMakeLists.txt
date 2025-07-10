
add_library(image-codec INTERFACE)
add_library(image::codec ALIAS image-codec)

add_subdirectory(module/jpeg)
target_link_libraries(
    image-codec
    INTERFACE
        image::codec::jpeg
)
