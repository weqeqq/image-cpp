
#include <image/buffer.h>
#include <image/capi/buffer.h>

namespace Image::CAPI {

Image::AlphaBuffer<> &BufferCast(image_buffer *buffer) {
  return *reinterpret_cast<Image::AlphaBuffer<> *>(buffer);
}
const Image::AlphaBuffer<> &BufferCast(const image_buffer *buffer) {
  return *reinterpret_cast<const Image::AlphaBuffer<> *>(buffer);
}

extern "C" {

image_buffer *image_buffer_new(uint64_t row_count, uint64_t column_count) {
  return reinterpret_cast<image_buffer *>(
    new Image::AlphaBuffer<>(row_count, column_count)
  );
}

image_buffer *image_buffer_copy(const image_buffer *buffer) {
  return reinterpret_cast<image_buffer *>(
    new Image::AlphaBuffer<>(*reinterpret_cast<const Image::AlphaBuffer<> *>(buffer))
  );
}

void image_buffer_delete(image_buffer *buffer) {
  delete &BufferCast(buffer);
}

uint64_t image_buffer_get_row_count(const image_buffer *buffer) {
  return BufferCast(buffer).GetRowCount();
}

uint64_t image_buffer_get_column_count(const image_buffer *buffer) {
  return BufferCast(buffer).GetColumnCount();
}

uint64_t image_buffer_get_length(const image_buffer *buffer) {
  return BufferCast(buffer).GetLength();
}

uint8_t *image_buffer_at(image_buffer *buffer, uint64_t row, uint64_t column) {
  return reinterpret_cast<uint8_t *>(
    &BufferCast(buffer)(row, column)
  );
}

uint8_t *image_buffer_at_index(image_buffer *buffer, uint64_t index) {
  return reinterpret_cast<uint8_t *>(
    &BufferCast(buffer)[index]
  );
}

const uint8_t *image_buffer_at_const(const image_buffer *buffer, uint64_t row, uint64_t column) {
  return reinterpret_cast<const uint8_t *>(
    &BufferCast(buffer)(row, column)
  );
}
const uint8_t *image_buffer_at_index_const(const image_buffer *buffer, uint64_t index) {
  return reinterpret_cast<const uint8_t *>(
    &BufferCast(buffer)[index]
  );
}
}
}

