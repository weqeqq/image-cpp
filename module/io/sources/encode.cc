
#ifdef IMAGE_FORMAT_JPEG

#include "image/codec/jpeg/color.h"
#include "image/codec/jpeg/sampling.h"
#include "image/io/error.h"
#include <image/io/encode.h>

#include <file/output.h>
#include <image/codec/jpeg.h>
// #include <image/codec/png.h>
// #include <image/codec/webp.h>

namespace Image {
//

std::vector<U8> EncodeJpeg(const Buffer<> &buffer) {
  Image::JpegCodec::Color color;
  switch (buffer.ChannelCount()) {
    case 1: color = Image::JpegCodec::Color::Gray; break;
    case 3: color = Image::JpegCodec::Color::Rgb;  break;
    case 4: color = Image::JpegCodec::Color::Rgba; break;
    default: {
      throw EncodeError("ChannelCountError");
    }
  }
  return Image::JpegCodec::Encode(
    buffer.Underlying(),
    buffer.RowCount(),
    buffer.ColumnCount(),
    color,
    Image::JpegCodec::Sampling::S444, 100);
}
std::vector<U8> Encode(
  const Buffer<> &buffer, Format format
) {
  switch (format) {
    case Format::Jpeg: return EncodeJpeg(buffer);
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
  const Buffer<> &buffer, const std::filesystem::path &path, Format format
) {
  return File::To(Encode(buffer, format), path);
}
void Encode(
  const Buffer<> &buffer, const std::filesystem::path &path
) {
  return Encode(buffer, path, DetectFormat(path));
}

}

#endif
