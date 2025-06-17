
#include <image/io/encode.h>

#include <image/codec/jpeg.h>
#include <image/codec/png.h>
#include <image/codec/webp.h>

#include <file/write.h>

namespace Image {

File::ByteSequence Encode(
  Buffer buffer, Format format
) {
  switch (format) {
    case Format::Png  : return Image::PngCodec  ::Encode(buffer.Data(), buffer.RowCount(), buffer.ColumnCount());
    case Format::Jpeg : return Image::JpegCodec ::Encode(buffer.Data(), buffer.RowCount(), buffer.ColumnCount());
    case Format::Webp : return Image::WebpCodec ::Encode(buffer.Data(), buffer.RowCount(), buffer.ColumnCount());
    default:
      throw EncodeError("Unsupported image format.");
  }
}
static bool IsJpeg(const std::filesystem::path filepath) {
  return filepath.extension() == ".jpg" ||
         filepath.extension() == ".jpeg";
}
static bool IsPng(const std::filesystem::path filepath) {
  return filepath.extension() == ".png";
}
static bool IsWebp(const std::filesystem::path filepath) {
  return filepath.extension() == ".webp";
}
static Format DetectFormat(const std::filesystem::path filepath) {
  if (IsJpeg(filepath)) {
    return Format::Jpeg;
  }
  if (IsPng(filepath)) {
    return Format::Png;
  }
  if (IsWebp(filepath)) {
    return Format::Webp;
  }
  throw EncodeError("Could not detect image format.");
}
void Encode(
  Buffer buffer, const std::filesystem::path &path, Format format
) {
  return File::Write(path.string(), Encode(buffer, format));
}
void Encode(
  Buffer buffer, const std::filesystem::path &path
) {
  return Encode(buffer, path, DetectFormat(path));
}

}
