
#pragma once

#include <image/core/endianness.h>

#include <cstdint>
#include <type_traits>

namespace Image {

template <typename T>
class EndiannessConvertor {
public:
  static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
  
  EndiannessConvertor(T input) : input_(input) {}

  template <Endianness::Tp endianness>
  T Convert() const {
    if constexpr (endianness == Endianness::Determine()) {
      return input_;
    } else {
      return Swap();
    }
  }
private:
  T input_;

  constexpr T Swap() const {
    if constexpr (sizeof(T) == 1) {
      return input_;
    } else if constexpr (sizeof(T) == 2) {
      std::uint16_t input = *reinterpret_cast<const std::uint16_t *>(&input_);
      return ((static_cast<std::uint64_t>(input) & 0xff00) >> 8) |
             ((static_cast<std::uint64_t>(input) & 0x00ff) << 8);
    } else if constexpr (sizeof(T) == 4) {
      std::uint32_t input = *reinterpret_cast<const std::uint32_t *>(&input_);
      return ((static_cast<std::uint64_t>(input) & 0xff000000) >> 24) |
             ((static_cast<std::uint64_t>(input) & 0x00ff0000) >> 8)  |
             ((static_cast<std::uint64_t>(input) & 0x0000ff00) << 8)  |
             ((static_cast<std::uint64_t>(input) & 0x000000ff) << 24);
    } else if constexpr (sizeof(T) == 8) {
      std::uint64_t input = *reinterpret_cast<const std::uint64_t *>(&input_);
      return ((static_cast<std::uint64_t>(input) & 0xff00000000000000) >> 56) | 
             ((static_cast<std::uint64_t>(input) & 0x00ff000000000000) >> 40) |
             ((static_cast<std::uint64_t>(input) & 0x0000ff0000000000) >> 24) |
             ((static_cast<std::uint64_t>(input) & 0x000000ff00000000) >> 8)  |
             ((static_cast<std::uint64_t>(input) & 0x00000000ff000000) << 8)  |
             ((static_cast<std::uint64_t>(input) & 0x0000000000ff0000) << 24) |
             ((static_cast<std::uint64_t>(input) & 0x000000000000ff00) << 40) |
             ((static_cast<std::uint64_t>(input) & 0x00000000000000ff) << 56);
    }
  }
}; // EndiannessConvertor

template <Endianness::Tp endianness, typename T>
static T ConvertEndianness(T input) {
  return EndiannessConvertor<T>(input).template Convert<endianness>();
}
  
}; // Image
