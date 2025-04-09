
#pragma once

#include <image/capi/buffer.h>
#include <image/capi/core/error.h>

#ifdef __cplusplus
namespace Image::CAPI {
extern "C" {
#endif 

image_error image_encode_to_file(const image_buffer *buffer, const char *file);

#ifdef __cplusplus
}
}
#endif
