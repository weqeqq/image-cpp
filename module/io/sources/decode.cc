
#ifdef IMAGE_FORMAT_JPEG

#include <image/io/decode.h>

#include <image/codec/jpeg.h>
#include <vector>
#include <file/input.h>
// #include <image/codec/png.h>
// #include <image/codec/webp.h>

namespace Image {
//
static bool CompareSignature(const std::vector<U8> &signature, const std::vector<U8> &data) {
  if (signature.size() > data.size()) {
    return false;
  }
  return std::equal(
    signature.begin(),
    signature.end(),
    data.begin()
  );
}
static bool IsJpeg(const std::vector<U8> &data) {
  return CompareSignature(
    std::vector<U8>{
      0xff, 0xd8, 0xff
    },
    data
  );
}
static bool IsPng(const std::vector<U8> &data) {
  return CompareSignature(
    std::vector<U8>{
      0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    },
    data
  );
}
static bool IsWebp(const std::vector<U8> &data) {
  return CompareSignature(
    std::vector<U8>{
      0x52, 0x49, 0x46, 0x46
    },
    data
  );
}
static Format DetectFormat(const std::vector<U8> &data) {
  if (IsJpeg(data)) {
    return Format::Jpeg;
  }
  if (IsPng(data)) {
    return Format::Png;
  }
  if (IsWebp(data)) {
    return Format::Webp;
  }
  throw DecodeError("Could not detect image format.");
}
static Buffer<> DecodeJpeg(const std::vector<U8> &data) {
  auto row_count = 0;
  auto col_count = 0;

  auto decoded = JpegCodec::Decode(data, row_count, col_count);
  return Buffer(
    std::move(decoded),
    row_count,
    col_count
  );
}
// static Buffer<> DecodePng(const std::vector<U8> &data) {
//   auto row_count = 0;
//   auto col_count = 0;

//   return Buffer(
//     row_count,
//     col_count,
//     PngCodec::Decode(data, row_count, col_count)
//   );
// }
// static Buffer<> DecodeWebp(const std::vector<U8> &data) {
//   auto row_count = 0;
//   auto col_count = 0;

//   return Buffer(
//     row_count,
//     col_count,
//     WebpCodec::Decode(data, row_count, col_count)
//   );
// }
Buffer<> Decode(const std::vector<U8> &data, Format format) {
  switch (format) {
    case Format::Jpeg : return DecodeJpeg(data);
    // case Format::Png  : return DecodePng  (data);
    // case Format::Webp : return DecodeWebp (data);
    default:
      throw DecodeError("Unsupported image format.");
  }
}
Buffer<> Decode(const std::vector<U8> &data) {
  return Decode(data, DetectFormat(data));
}
Buffer<> Decode(const std::filesystem::path &path, Format format) {
  return Decode(File::From(path), format);
}
Buffer<> Decode(const std::filesystem::path &path) {
  return Decode(File::From(path));
}
}

#endif
