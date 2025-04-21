
#include <image/decode.h>
#include <turbojpeg.h>

namespace Image::DecodeImpl {

class Wrapper {
public:
  explicit Wrapper(const std::vector<std::uint8_t> &input) : input_(input) {}

  template <bool AlphaSetting>
  Buffer<DefDepth, DefColor, AlphaSetting> Decode() const {
    std::int32_t row_count, column_count, subsamp;

    tjhandle decompressor = tjInitDecompress();
    if (tjDecompressHeader2(decompressor, const_cast<std::uint8_t *>(input_.data()), input_.size(), &column_count, &row_count, &subsamp)) {
      throw std::runtime_error("failed to decode header");
    }

    std::vector<std::uint8_t> buffer(row_count * column_count * tjPixelSize[AlphaSetting == EnableAlpha ? TJPF_RGBA : TJPF_RGB]);
    if (tjDecompress2(decompressor, const_cast<std::uint8_t *>(input_.data()), input_.size(), buffer.data(), column_count, 0, row_count, AlphaSetting == EnableAlpha ? TJPF_RGBA : TJPF_RGB, 0)) {
      throw std::runtime_error("failed to decode image");
    }
    tjDestroy(decompressor);
    return SequenceConvertor(buffer).template Convert<DefDepth, DefColor, AlphaSetting>(row_count, column_count);
  }
private:
  const std::vector<std::uint8_t> &input_;
};
template <>
Buffer<DefDepth, DefColor, EnableAlpha> DecodeJPEG(const std::vector<std::uint8_t> &input) {
  return Wrapper(input).template Decode<EnableAlpha>();
}
template <>
Buffer<DefDepth, DefColor, DisableAlpha> DecodeJPEG(const std::vector<std::uint8_t> &input) {
  return Wrapper(input).template Decode<DisableAlpha>();
}
}
