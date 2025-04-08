
#pragma once

#include <image/capi/buffer.h>
#include <image/capi/core/error.h>

#ifdef __cplusplus
namespace Image::CAPI {
extern "C" {
#endif 

image_error image_decode_buffer_from_file(image_buffer *output, const char *file);

#ifdef __cplusplus
}
}
#endif

