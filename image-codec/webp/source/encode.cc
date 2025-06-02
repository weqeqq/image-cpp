
#include <image/codec/webp/encode.h>

#include <webp/encode.h>

namespace Image::WebpCodec {

File::ByteSequence EncodeBgr(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  std::int32_t   qual
) {
  std::uint8_t *pointer = nullptr;
  
  if (qual == 100) {
    if (!WebPEncodeLosslessBGR(
      data.data(), 
      column_count,
      row_count,
      column_count * 3,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    };
  } else {
    if (!WebPEncodeBGR(
      data.data(), 
      column_count,
      row_count,
      qual,
      column_count * 3,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    };
  }
  File::ByteSequence output(
    pointer, 
    pointer + row_count * column_count * 3
  );
  WebPFree(pointer);

  return output;
}
File::ByteSequence EncodeBgra(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  std::int32_t   qual
) {
  std::uint8_t *pointer = nullptr;
  
  if (qual == 100) {
    if (!WebPEncodeLosslessBGRA(
      data.data(), 
      column_count,
      row_count,
      column_count * 4,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    }
  } else {
    if (!WebPEncodeBGRA(
      data.data(), 
      column_count,
      row_count,
      qual,
      column_count * 4,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    }
  }
  File::ByteSequence output(
    pointer, 
    pointer + row_count * column_count * 4
  );
  WebPFree(pointer);

  return output;
}
File::ByteSequence EncodeRgb(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  std::int32_t   qual
) {
  std::uint8_t *pointer = nullptr;
  
  if (qual == 100) {
    if (!WebPEncodeLosslessRGB(
      data.data(), 
      column_count,
      row_count,
      column_count * 3,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    }
  } else {
    if (!WebPEncodeRGB(
      data.data(), 
      column_count,
      row_count,
      qual,
      column_count * 3,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    }
  }
  File::ByteSequence output(
    pointer, 
    pointer + row_count * column_count * 3
  );
  WebPFree(pointer);

  return output;
}
File::ByteSequence EncodeRgba(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  std::int32_t   qual
) {
  std::uint8_t *pointer = nullptr;
  
  if (qual == 100) {
    if (!WebPEncodeLosslessRGBA(
      data.data(), 
      column_count,
      row_count,
      column_count * 4,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    }
  } else {
    if (!WebPEncodeRGBA(
      data.data(), 
      column_count,
      row_count,
      qual,
      column_count * 4,
      &pointer
    )) {
      throw EncodeError("failed to encode image");
    }
  }
  File::ByteSequence output(
    pointer, 
    pointer + row_count * column_count * 4
  );
  WebPFree(pointer);

  return output;
}
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  Color          color,
  std::int32_t   qual
) {
  switch (color) {
    case Color::Bgr  : return EncodeBgr  (data, row_count, column_count, qual);
    case Color::Bgra : return EncodeBgra (data, row_count, column_count, qual);
    case Color::Rgb  : return EncodeRgb  (data, row_count, column_count, qual);
    case Color::Rgba : return EncodeRgba (data, row_count, column_count, qual);
    default:
      throw EncodeError("unsupported color format"); 
  }
}
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count
) {
  return Encode(
    data,
    row_count,
    column_count,
    Color::Rgba,
    100
  );
}
}
