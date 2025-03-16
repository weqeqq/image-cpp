
cmake_minimum_required(VERSION 3.23)

project(image-test VERSION 0.1.0 LANGUAGES CXX)

add_executable(image-test)

set_target_properties(image-test PROPERTIES 
  CXX_STANDARD          17
  CXX_STANDARD_REQUIRED 1 
  CXX_EXTENSIONS        0)

target_sources(image-test PRIVATE 
  source/convertor/bitmap_test.cc 
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0)

FetchContent_MakeAvailable(googletest)

target_link_libraries(image-test PRIVATE image::image)
target_link_libraries(image-test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(image-test)
