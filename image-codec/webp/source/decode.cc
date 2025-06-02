
#include <image/codec/webp/decode.h>

#include <webp/decode.h>

namespace Image::WebpCodec {

static File::ByteSequence DecodeBgr(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
) {
  auto pointer = WebPDecodeBGR(
    data.data(),
    data.size(),
    &column_count,
    &row_count
  );
  if (pointer == nullptr) {
    throw DecodeError("failed to decode image");
  }
  auto output = File::ByteSequence(
    pointer,
    pointer + row_count * column_count * 3
  );
  WebPFree(pointer);
  return output;
}
static File::ByteSequence DecodeBgra(
  File::ByteSpan data,
  std::int32_t &row_count,
  std::int32_t &column_count
) {
  auto pointer = WebPDecodeBGRA(
    data.data(),
    data.size(),
    &column_count,
    &row_count
  );
  if (pointer == nullptr) {
    throw DecodeError("failed to decode image");
  }
  auto output = File::ByteSequence(
    pointer,
    pointer + row_count * column_count * 4
  );
  WebPFree(pointer);
  return output;
}
static File::ByteSequence DecodeRgb(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
) {
  auto pointer = WebPDecodeRGB(
    data.data(),
    data.size(),
    &column_count,
    &row_count
  );
  if (pointer == nullptr) {
    throw DecodeError("failed to decode image");
  }
  auto output = File::ByteSequence(
    pointer,
    pointer + row_count * column_count * 3
  );
  WebPFree(pointer);
  return output;
}
static File::ByteSequence DecodeRgba(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
) {
  auto pointer = WebPDecodeRGBA(
    data.data(),
    data.size(),
    &column_count,
    &row_count
  );
  if (pointer == nullptr) {
    throw DecodeError("failed to decode image");
  }
  auto output = File::ByteSequence(
    pointer,
    pointer + row_count * column_count * 4
  );
  WebPFree(pointer);
  return output;
}
static File::ByteSequence DecodeArgb(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
) {
  auto pointer = WebPDecodeARGB(
    data.data(),
    data.size(),
    &column_count,
    &row_count
  );
  if (pointer == nullptr) {
    throw DecodeError("failed to decode image");
  }
  auto output = File::ByteSequence(
    pointer,
    pointer + row_count * column_count * 4
  );
  WebPFree(pointer);
  return output;
}

File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count,
  Color          color
) {
  switch (color) {
    case Color::Bgr  : return DecodeBgr  (data, row_count, column_count);
    case Color::Bgra : return DecodeBgra (data, row_count, column_count);
    case Color::Rgb  : return DecodeRgb  (data, row_count, column_count);
    case Color::Rgba : return DecodeRgba (data, row_count, column_count);
    case Color::Argb : return DecodeArgb (data, row_count, column_count);
    default: 
      throw DecodeError("unsupported color format");
  }
}
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
) {
  return Decode(
    data,
    row_count,
    column_count,
    Color::Rgba
  );
}
}
