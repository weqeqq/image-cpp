
include(FetchContent)

FetchContent_Declare(
  file-project
  GIT_REPOSITORY https://github.com/weqeqq/file-cpp.git
  GIT_TAG        06108062077d0d6abd8550988fd8cf5a9f5d3d11
  GIT_PROGRESS   ON
)
FetchContent_MakeAvailable(
  file-project
)
add_library(target::file ALIAS file)
