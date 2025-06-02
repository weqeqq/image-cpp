
include(FetchContent)

FetchContent_Declare(
  file-project
  GIT_REPOSITORY https://github.com/weqeqq/file-cpp.git
  GIT_TAG        964b7e842aa36b8fa40be0608109029f76062fa1
  GIT_PROGRESS   ON
)
FetchContent_MakeAvailable(
  file-project
)
add_library(target::file ALIAS file)
