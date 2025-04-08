
#pragma once

#include <image/capi/core/error.h>

#ifdef __cplusplus

#include <image/buffer.h>
namespace Image::CAPI {

extern "C" {
#endif 

#include <stdint.h>

typedef struct image_buffer image_buffer;

image_buffer *image_buffer_new(uint64_t row_count, uint64_t column_count);
image_buffer *image_buffer_copy(const image_buffer *buffer);

void image_buffer_delete(image_buffer *buffer);

uint64_t image_buffer_get_row_count    (const image_buffer *buffer);
uint64_t image_buffer_get_column_count (const image_buffer *buffer);

uint64_t image_buffer_get_length(const image_buffer *buffer);

uint8_t *image_buffer_at       (image_buffer *buffer, uint64_t row, uint64_t column);
uint8_t *image_buffer_at_index (image_buffer *buffer, uint64_t index);

const uint8_t *image_buffer_at_const       (const image_buffer *buffer, uint64_t row, uint64_t column);
const uint8_t *image_buffer_at_index_const (const image_buffer *buffer, uint64_t index);

#ifdef __cplusplus
}

Image::AlphaBuffer<> &BufferCast(image_buffer *buffer);
const Image::AlphaBuffer<> &BufferCast(const image_buffer *buffer);

}
#endif

