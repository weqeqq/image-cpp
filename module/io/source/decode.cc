
#include <image/io/decode.h>

#include <image/codec/jpeg.h>
#include <image/codec/png.h>
#include <image/codec/webp.h>

namespace Image {

static bool CompareSignature(File::ByteSpan signature, File::ByteSpan data) {
  if (signature.size() > data.size()) {
    return false;
  }
  return std::equal(
    signature.begin(),
    signature.end(),
    data.begin()
  );
}
static bool IsJpeg(File::ByteSpan data) {
  return CompareSignature(
    File::ByteSequence{
      0xff, 0xd8, 0xff
    },
    data
  );
}
static bool IsPng(File::ByteSpan data) {
  return CompareSignature(
    File::ByteSequence{
      0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    }, 
    data
  );
}
static bool IsWebp(File::ByteSpan data) {
  return CompareSignature(
    File::ByteSequence{
      0x52, 0x49, 0x46, 0x46
    }, 
    data
  );
}
static Format DetectFormat(File::ByteSpan data) {
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
static Buffer DecodeJpeg(File::ByteSpan data) {
  auto row_count    = 0;
  auto col_count = 0;

  return Buffer(
    row_count, 
    col_count, 
    JpegCodec::Decode(data, row_count, col_count)
  );
}
static Buffer DecodePng(File::ByteSpan data) {
  auto row_count = 0;
  auto col_count = 0;

  return Buffer(
    row_count, 
    col_count, 
    PngCodec::Decode(data, row_count, col_count)
  );
}
static Buffer DecodeWebp(File::ByteSpan data) {
  auto row_count = 0;
  auto col_count = 0;

  return Buffer(
    row_count, 
    col_count, 
    WebpCodec::Decode(data, row_count, col_count)
  );
}
Buffer Decode(File::ByteSpan data, Format format) {
  switch (format) {
    case Format::Jpeg : return DecodeJpeg (data);
    case Format::Png  : return DecodePng  (data);
    case Format::Webp : return DecodeWebp (data);
    default: 
      throw DecodeError("Unsupported image format.");
  }
}
Buffer Decode(File::ByteSpan data) {
  return Decode(data, DetectFormat(data));
}
Buffer Decode(const std::filesystem::path &path, Format format) {
  return Decode(File::Read(path.string()), format);
}
Buffer Decode(const std::filesystem::path &path) {
  return Decode(File::Read(path.string()));
}
}
