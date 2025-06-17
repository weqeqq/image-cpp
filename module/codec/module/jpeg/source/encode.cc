
#include <image/codec/jpeg/encode.h>
#include <turbojpeg.h>

namespace Image::JpegCodec {

constexpr auto Pitch = 0;
constexpr auto Flags = 0;

File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count, 
  std::int32_t   column_count, 
  Color          color,
  Sampling       sampling,
  int            qual 
) {
  auto compressor = tjInitCompress();

  unsigned char *buffer = nullptr;
  unsigned long  length = 0;
  
  if (tjCompress2(
    compressor,
    const_cast<std::uint8_t *>(data.data()),
    column_count,
    Pitch,
    row_count,
    static_cast<TJPF>(color),
    &buffer,
    &length,
    static_cast<TJSAMP>(sampling), 
    qual,
    Flags
  )) {
    throw EncodeError("failed to encode image");
  }
  File::ByteSequence output(buffer, buffer + length);

  tjDestroy(compressor);
  tjFree(buffer);

  return output;
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
    Color::Rgb,
    Sampling::S444, 
    100
  );
}

}
