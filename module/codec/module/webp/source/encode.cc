#include <image/codec/webp/encode.h>

#include <webp/encode.h>

namespace Image::WebpCodec {
namespace {

template <typename EncoderFunc, typename LosslessEncoderFunc>
File::ByteSequence EncodeWebP(
  File::ByteSpan      data,
  std::int32_t        row_count,
  std::int32_t        col_count,
  std::int32_t        stride,
  std::int32_t        qual,
  EncoderFunc         encoder,
  LosslessEncoderFunc lossless_encoder
) {
  if (data.empty() || 
      col_count <= 0 || 
      row_count <= 0 || 
      stride    <= 0) {
    throw EncodeError("Invalid input parameters");
  }
  if (qual < 0 || qual > 100) {
    throw EncodeError("Quality must be between 0 and 100");
  }
  std::uint8_t* buffer = nullptr;
  std::uint64_t length = 0;

  if (qual == 100) {
    length = lossless_encoder(
      data.data(), col_count, row_count, stride, &buffer
    );
  } else {
    length = encoder(
      data.data(), col_count, row_count, stride, qual, &buffer
    );
  }
  if (length == 0 || buffer == nullptr) {
    throw EncodeError("Failed to encode image");
  }
  File::ByteSequence output(
    buffer, 
    buffer + length
  );
  WebPFree(buffer);

  return output;
}
File::ByteSequence EncodeBgr(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   col_count,
  std::int32_t   qual
) {
  return EncodeWebP(
    data, row_count, col_count, col_count * 3, qual,
    WebPEncodeBGR,
    WebPEncodeLosslessBGR
  );
}
File::ByteSequence EncodeBgra(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   col_count,
  std::int32_t   qual
) {
  return EncodeWebP(
    data, row_count, col_count, col_count * 4, qual,
    WebPEncodeBGRA,
    WebPEncodeLosslessBGRA
  );
}
File::ByteSequence EncodeRgb(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   col_count,
  std::int32_t   qual
) {
  return EncodeWebP(
    data, row_count, col_count, col_count * 3, qual,
    WebPEncodeRGB,
    WebPEncodeLosslessRGB
  );
}
File::ByteSequence EncodeRgba(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   col_count,
  std::int32_t   qual
) {
  return EncodeWebP(
    data, row_count, col_count, col_count * 4, qual,
    WebPEncodeRGBA,
    WebPEncodeLosslessRGBA
  );
}
} 
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   col_count,
  Color          color,
  std::int32_t   qual
) {
  switch (color) {
    case Color::Bgr:  return EncodeBgr  (data, row_count, col_count, qual);
    case Color::Bgra: return EncodeBgra (data, row_count, col_count, qual);
    case Color::Rgb:  return EncodeRgb  (data, row_count, col_count, qual);
    case Color::Rgba: return EncodeRgba (data, row_count, col_count, qual);
    default:
      throw EncodeError("unsupported color format");
  }
}
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   col_count
) {
  return Encode(
    data, row_count, col_count, Color::Rgba, 100
  );
}
} 
