#pragma once

#include <image/buffer.h>
#include <image/convertor/depth.h>
#include <image/convertor/color.h>
#include <image/convertor/sequence.h>
#include <fstream>
#include <filesystem>
#include <optional>

#include <image/core/format.h>

namespace Image {
namespace EncodeImpl {

template <bool AlphaSetting>
Sequence EncodePNG(const Buffer<DefDepth, DefColor, AlphaSetting> &input) {
  static_assert(false);

  return Sequence();
}
template <>
Sequence EncodePNG<EnableAlpha>(const Buffer<DefDepth, DefColor, EnableAlpha> &input);

template <>
Sequence EncodePNG<DisableAlpha>(const Buffer<DefDepth, DefColor, DisableAlpha> &input);

template <bool AlphaSetting>
Sequence EncodeJPEG( const Buffer<DefDepth, DefColor, AlphaSetting> &input) {
  static_assert(false);

  return Sequence();
}
template <>
Sequence EncodeJPEG<EnableAlpha>(const Buffer<DefDepth, DefColor, EnableAlpha> &input);

template <>
Sequence EncodeJPEG<DisableAlpha>(const Buffer<DefDepth, DefColor, DisableAlpha> &input);

template <bool AlphaSetting>
Sequence EncodeWebP(const Buffer<DefDepth, DefColor, AlphaSetting> &input) {
  static_assert(false);

  return Sequence();
}
template <>
Sequence EncodeWebP<EnableAlpha>(const Buffer<DefDepth, DefColor, EnableAlpha> &input);

template <>
Sequence EncodeWebP<DisableAlpha>(const Buffer<DefDepth, DefColor, DisableAlpha> &input);
};
template <Depth::Tp DepthV, 
          Color::Tp ColorV, 
          bool AlphaSetting>
Sequence Encode(const Buffer<DepthV, ColorV, AlphaSetting> &input, Format::Tp format) {
  decltype(auto) data = DataConvertor(input).template Convert<DefDepth, DefColor>();

  switch (format) {
    case Format::PNG  : return EncodeImpl::EncodePNG  (data);
    case Format::JPEG : return EncodeImpl::EncodeJPEG (data);

    default: throw std::runtime_error("UndefinedFormat");
  }
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV, 
          bool AlphaSetting>
void Encode(const Buffer<DepthV, ColorV, AlphaSetting> &input, std::filesystem::path path) {
  std::ofstream stream(path, std::ios::binary);

  if (!stream.is_open()) {
    throw std::runtime_error("Failed to open output file stream");
  }
  auto buffer = Encode(input, Format::FromPath(path));

  stream.write(reinterpret_cast<char *>(buffer.data()), buffer.size());
  stream.close();
}
}
