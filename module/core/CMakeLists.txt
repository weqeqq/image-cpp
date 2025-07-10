

add_library(image-core INTERFACE)
add_library(image::core ALIAS image-core)

target_sources(image-core
    INTERFACE
        FILE_SET HEADERS BASE_DIRS
            headers
        FILES
            headers/image/core/error.h
            headers/image/core/buffer.h
)
