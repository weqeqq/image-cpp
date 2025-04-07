#pragma once

#include <cstdint>

namespace Image {
class Color {
public:
  enum Tp {
    Bitmap,
    Grayscale,
    Indexed,
    RGB,
    CMYK,
    Multichannel,
    Duotone,
    Lab,
  };

private:
  template <Tp>
  struct ChannelCountS;

public:
  static constexpr std::uint64_t AlphaChannel = 1;

  template <Tp Value, bool AlphaSetting>
  static constexpr std::uint64_t ChannelCount = AlphaSetting 
    ? ChannelCountS<Value>::value + AlphaChannel
    : ChannelCountS<Value>::value;
};

template <>
struct Color::ChannelCountS<Color::Grayscale> {
  static constexpr std::uint64_t value = 1;
};

template <>
struct Color::ChannelCountS<Color::RGB> {
  static constexpr std::uint64_t value = 3;
};

template <>
struct Color::ChannelCountS<Color::CMYK> {
  static constexpr std::uint64_t value = 4;
};

static constexpr Color::Tp DefColor = Color::RGB;

};

