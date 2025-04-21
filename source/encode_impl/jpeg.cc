
#include <image/encode.h>
#include <turbojpeg.h>

namespace Image::EncodeImpl {

template <bool AlphaSetting>
class Wrapper {
public:
  using UsedBuffer = Buffer<DefDepth, DefColor, AlphaSetting>;

  explicit Wrapper(const UsedBuffer &input) : input_(input) {}

  auto Encode() const {
    Sequence output;

    tjhandle compressor = tjInitCompress();

    unsigned char *jpeg_buffer = nullptr;
    unsigned long  jpeg_length = 0; 

    tjCompress2(
      compressor, 
      SequenceConvertor(input_).Convert().data(), 
      input_.GetColumnCount(), 
      0, 
      input_.GetRowCount(),
      AlphaSetting == EnableAlpha ? TJPF_RGBA : TJPF_RGB,
      &jpeg_buffer,
      &jpeg_length,
      TJSAMP_444, 100, 0
    );
    output.insert(output.end(), jpeg_buffer, jpeg_buffer + jpeg_length);

    tjDestroy(compressor);
    tjFree(jpeg_buffer);

    return output;
  }

private:
  const UsedBuffer &input_;
};
template <>
Sequence EncodeJPEG<EnableAlpha>(const Buffer<DefDepth, DefColor, EnableAlpha> &input) {
  return Wrapper<EnableAlpha>(input).Encode();
}
template <>
Sequence EncodeJPEG<DisableAlpha>(const Buffer<DefDepth, DefColor, DisableAlpha> &input) {
  return Wrapper<DisableAlpha>(input).Encode();
}
}
