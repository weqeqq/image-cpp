
set(name image-core-test)

add_executable(${name})

target_sources(${name} PRIVATE
  source/buffer_test.cc
)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${name} PRIVATE image::core)
target_link_libraries(${name} PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${name})
