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
    Lab
  };

private:
  template <Tp>
  struct ChannelCountS;

public:
  template <Tp V>
  static constexpr std::uint64_t ChannelCount = ChannelCountS<V>::value;
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
};

