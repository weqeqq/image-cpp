
#include <image/decode.h>
#include <image/capi/decode.h>

namespace Image::CAPI {
extern "C" {

image_error image_decode_from_file(image_buffer *output, const char *file) {
  try {
    output = reinterpret_cast<image_buffer *>(
      new Image::AlphaBuffer<>(Image::Decode(file))
    );
  } catch (std::exception &err) {
    return { EXIT_FAILURE, err.what() };
  }
  return { EXIT_SUCCESS, nullptr };
}
}
}
