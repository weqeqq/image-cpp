
#include <image/encode.h>

#include <fpng.h>

namespace Image::EncodeImpl {

template <>
Sequence EncodePNG<EnableAlpha>(const Buffer<DefDepth, DefColor, EnableAlpha> &input) {
  Sequence output;
  fpng::fpng_encode_image_to_memory(SequenceConvertor(input).Convert().data(), input.GetColumnCount(), input.GetRowCount(), 4, output); 

  return output;
}
template <>
Sequence EncodePNG<DisableAlpha>(const Buffer<DefDepth, DefColor, DisableAlpha> &input) {
  Sequence output;
  fpng::fpng_encode_image_to_memory(SequenceConvertor(input).Convert().data(), input.GetColumnCount(), input.GetRowCount(), 3, output); 

  return output;
}
};
