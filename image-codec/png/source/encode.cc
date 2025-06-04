
#include <image/codec/png/encode.h>

#include <fpng.h>

namespace Image::PngCodec {

File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  Color          color
) {
  auto channel_count = 0;

  switch (color) {
    case Color::Rgba : channel_count = 4; break;
    case Color::Rgb  : channel_count = 3; break;
    default: {
      throw EncodeError("Unsupported color");
    }
  }
  File::ByteSequence output;

  fpng::fpng_encode_image_to_memory(
    data.data(),
    column_count,
    row_count,
    channel_count,
    output
  ) ;
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
    Color::Rgba
  );
}
}
