
#include <image/encode.h>
#include <image/capi/encode.h>

namespace Image::CAPI {
extern "C" {

image_error image_encode_buffer_to_file(const image_buffer *buffer, const char *file) {
  try {
    Encode(BufferCast(buffer), file);

  } catch (std::exception &err) {
    return { EXIT_FAILURE, err.what() };
  }
  return { EXIT_SUCCESS, nullptr };
}
}
}
