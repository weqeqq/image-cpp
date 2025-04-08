
#include <image/decode.h>
#include <image/convertor/sequence.h>
#include <wuffs.h>

namespace Image::DecodeImpl {

class Wrapper {
public:
  explicit Wrapper(const std::vector<std::uint8_t> &input) : input_(input) {}

  template <bool AlphaSetting>
  Buffer<DefDepth, DefColor, AlphaSetting> Decode() const {

    std::uint32_t row_count    = 0;
    std::uint32_t column_count = 0;

    return SequenceConvertor(
      Decode<AlphaSetting>(row_count, column_count)
    ).template Convert<DefDepth, DefColor, AlphaSetting>(row_count, column_count);
  }

private:
  const std::vector<std::uint8_t> &input_;

  template <bool AlphaSetting>
  Sequence Decode(std::uint32_t &row_count, std::uint32_t &column_count) const {
    std::unique_ptr<wuffs_png__decoder, decltype(&free)> png_decoder(wuffs_png__decoder__alloc(), &free);
    if (!png_decoder) {
      throw std::runtime_error("Failed to allocate PNG decoder");
    }
    wuffs_png__decoder__set_quirk(png_decoder.get(), WUFFS_BASE__QUIRK_IGNORE_CHECKSUM, true);

    wuffs_base__image_config image_config;
    wuffs_base__io_buffer source = wuffs_base__ptr_u8__reader(const_cast<std::uint8_t*>(input_.data()), input_.size(), true);
    wuffs_base__status status = wuffs_png__decoder__decode_image_config(png_decoder.get(), &image_config, &source);
    if (status.repr) {
      throw std::runtime_error("Failed to decode PNG image config: " + std::string(status.repr));
    }
    row_count    = wuffs_base__pixel_config__height(&image_config.pixcfg);
    column_count = wuffs_base__pixel_config__width(&image_config.pixcfg);

    wuffs_base__pixel_config__set(
      &image_config.pixcfg,
      AlphaSetting == EnableAlpha 
        ? WUFFS_BASE__PIXEL_FORMAT__RGBA_NONPREMUL
        : WUFFS_BASE__PIXEL_FORMAT__RGB, 
      WUFFS_BASE__PIXEL_SUBSAMPLING__NONE, column_count, row_count
    );
    uint64_t workbuf_len = wuffs_png__decoder__workbuf_len(png_decoder.get()).max_incl;
    if (workbuf_len > SIZE_MAX) {
      throw std::runtime_error("Work buffer size exceeds maximum allowed size");
    }

    std::unique_ptr<std::uint8_t[]> workbuf(new std::uint8_t[workbuf_len]);
    wuffs_base__slice_u8 workbuf_slice = wuffs_base__make_slice_u8(workbuf.get(), workbuf_len);

    const std::uint64_t total_pixels = static_cast<std::uint64_t>(row_count) * column_count;
    if (total_pixels > (SIZE_MAX >> 2U)) {
      throw std::runtime_error("Image dimensions too large, potential overflow");
    }
    std::vector<std::uint8_t> png_buffer(total_pixels * Color::ChannelCount<DefColor, AlphaSetting>);
    wuffs_base__slice_u8 pixbuf_slice = wuffs_base__make_slice_u8(png_buffer.data(), png_buffer.size());

    wuffs_base__pixel_buffer pixel_buffer;
    status = wuffs_base__pixel_buffer__set_from_slice(&pixel_buffer, &image_config.pixcfg, pixbuf_slice);
    if (status.repr) {
      throw std::runtime_error("Failed to set pixel buffer: " + std::string(status.repr));
    }
    status = wuffs_png__decoder__decode_frame(png_decoder.get(), &pixel_buffer, &source, WUFFS_BASE__PIXEL_BLEND__SRC, workbuf_slice, nullptr);
    if (status.repr) {
      throw std::runtime_error("Failed to decode PNG frame: " + std::string(status.repr));
    }
    return png_buffer;
  }
};
template <>
Buffer<DefDepth, DefColor, EnableAlpha> DecodePNG(const Sequence &input) {
  return Wrapper(input).template Decode<EnableAlpha>();
}
template <>
Buffer<DefDepth, DefColor, DisableAlpha> DecodePNG(const Sequence &input) {
  return Wrapper(input).template Decode<DisableAlpha>();
}
};
