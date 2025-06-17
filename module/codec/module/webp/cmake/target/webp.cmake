
include(FetchContent)

FetchContent_Declare(
  webp-project
  GIT_REPOSITORY https://github.com/webmproject/libwebp.git
  GIT_TAG        0cd0b7a7013723985156989f0772e3cb8c4ce49f
  GIT_PROGRESS   ON
)
FetchContent_MakeAvailable(
  webp-project
)
add_library(target::webp ALIAS webp)
