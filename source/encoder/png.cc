
#include <image/encoder.h>

#include <fpng.h>
#include <turbojpeg.h>

namespace Image::_Encoder {

template <>
void EncodePNG<Depth::Eight, Color::RGB>(
  const Buffer<Depth::Eight, Color::RGB>  &input, 
  const Buffer<Depth::Eight, Color::Grayscale> &alpha, std::vector<std::uint8_t> &output
) {
  fpng::fpng_encode_image_to_memory(SequenceConvertor(input).ToSequence(alpha).data(), input.GetCCount(), input.GetRCount(), 4, output); 
}
template <>
void EncodePNG<Depth::Eight, Color::RGB>(
  const Buffer<Depth::Eight, Color::RGB> &input, std::vector<std::uint8_t> &output
) {
  fpng::fpng_encode_image_to_memory(SequenceConvertor(input).ToSequence().data(), input.GetCCount(), input.GetRCount(), 3, output);
}
}; // Image::_Encoder
