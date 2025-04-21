#pragma once

#include <image/buffer.h>
#include <cstring>
#include <image/convertor/sequence.h>
#include <cerrno>
#include <cstdlib>
#include <image/convertor/depth.h>
#include <image/convertor/color.h>
#include <image/convertor/data.h>
#include <fstream>
#include <filesystem>
#include <image/core/format.h>

namespace Image {
namespace DecodeImpl {

static bool IsPNG(const Sequence &input) {
  Sequence signature = { 
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
  };
  if (input.size() < signature.size()) {
    return false;
  }
  return std::equal(
    signature.begin(), 
    signature.end(),
    input.begin()
  );
}
template <bool AlphaSetting>
Buffer<DefDepth, DefColor, AlphaSetting> DecodePNG(const Sequence &input) {
  static_assert(false);
}
template <>
Buffer<DefDepth, DefColor, EnableAlpha> DecodePNG(const Sequence &input);

template <>
Buffer<DefDepth, DefColor, DisableAlpha> DecodePNG(const Sequence &input);

static bool IsJPEG(const Sequence &input) {
  Sequence signature = { 
    0xFF, 0xD8, 0xFF 
  };
  if (input.size() < signature.size()) {
    return false;
  }
  return std::equal(
    signature.begin(), 
    signature.end(),
    input.begin()
  );
}
template <bool AlphaSetting>
Buffer<DefDepth, DefColor, AlphaSetting> DecodeJPEG(const Sequence &input) {
  static_assert(false);
}
template <>
Buffer<DefDepth, DefColor, EnableAlpha> DecodeJPEG(const Sequence &input);

template <>
Buffer<DefDepth, DefColor, DisableAlpha> DecodeJPEG(const Sequence &input);

static bool IsWebP(const Sequence &input) {
  Sequence signature = { 
    0x52, 0x49, 0x46, 0x46
  };
  if (input.size() < signature.size()) {
    return false;
  }
  return std::equal(
    signature.begin(), 
    signature.end(),
    input.begin()
  );
}
template <bool AlphaSetting>
Buffer<DefDepth, DefColor, AlphaSetting> DecodeWebP(const Sequence &input) {
  static_assert(false);
}
template <>
Buffer<DefDepth, DefColor, EnableAlpha> DecodeWebP(const Sequence &input);

template <>
Buffer<DefDepth, DefColor, DisableAlpha> DecodeWebP(const Sequence &input);
}
template <Depth::Tp DepthV  = DefDepth, 
          Color::Tp ColorV  = DefColor,
          bool AlphaSetting = EnableAlpha>
auto Decode(const Sequence &input) {
  Buffer<DefDepth, DefColor, AlphaSetting> output;

  if (DecodeImpl::IsPNG(input)) {
    output = DecodeImpl::DecodePNG<AlphaSetting>(input);

  } else if (DecodeImpl::IsJPEG(input)) {
    output = DecodeImpl::DecodeJPEG<AlphaSetting>(input);

  // } else if (DecodeImpl::IsWebP(input)) {
  //   output = DecodeImpl::DecodeWebP<AlphaSetting>(input);

  } else {
    throw std::runtime_error("Unrecognized image format");
  }
  return DataConvertor(output).template Convert<DepthV, ColorV>();
}
template <Depth::Tp DepthV  = DefDepth, 
          Color::Tp ColorV  = DefColor,
          bool AlphaSetting = EnableAlpha>
auto Decode(const std::filesystem::path &path) {
  std::ifstream stream(path, std::ios::binary);
  if (!stream.is_open()) {
    throw std::runtime_error(std::string("failed to open file: ") + std::strerror(errno));
  }
  stream.seekg(0, std::ios::end);
  std::uint64_t length = stream.tellg();
  stream.seekg(0, std::ios::beg);

  Sequence buffer(length);
  if (!stream.read(reinterpret_cast<char *>(buffer.data()), length)) {
    throw std::runtime_error("failed to read stream");
  }
  stream.close();
  return Decode(buffer);
}
}

