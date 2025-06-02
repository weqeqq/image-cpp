
add_executable(
  image-codec-jpeg-test
)
include(
  cmake/target/gtest.cmake
)
target_link_libraries(image-codec-jpeg-test
  PRIVATE
    target::gtest
    image::codec::jpeg
)
target_sources(image-codec-jpeg-test
  PRIVATE 
    source/decoder_test.cc
    source/encoder_test.cc
)
enable_testing()
include(GoogleTest)

gtest_discover_tests(
  image-codec-jpeg-test
)
