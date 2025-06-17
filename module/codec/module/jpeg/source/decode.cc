
#include <image/codec/jpeg/decode.h>
#include <turbojpeg.h>

namespace Image::JpegCodec {

constexpr auto Pitch = 0;
constexpr auto Flags = 0;

File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count,
  Color          color,
  Sampling      &sampling
) {
  auto decompressor = tjInitDecompress();

  if (tjDecompressHeader2(
    decompressor,
    const_cast<std::uint8_t *>(data.data()),
    data.size(),
    &column_count,
    &row_count,
    reinterpret_cast<int *>(&sampling)
  )) {
    throw DecodeError("failed to decode header");
  }
  File::ByteSequence output(
    row_count    * 
    column_count * 
    tjPixelSize[static_cast<TJPF>(color)]
  );
  if (tjDecompress2(
      decompressor,
      const_cast<std::uint8_t *>(data.data()),
      data.size(),
      output.data(),
      column_count,
      Pitch,
      row_count,
      static_cast<TJPF>(color),
      Flags
  )) {
    throw DecodeError("failed to decode image");
  }
  tjDestroy(decompressor);
  return output;
}
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
) {
  Sampling sampling;
  return Decode(
    data, 
    row_count, 
    column_count, 
    Color::Rgba, 
    sampling
  );
}
}
