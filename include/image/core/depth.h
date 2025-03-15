#pragma once

#include <cstdint>
#include <limits>

namespace Image {
class Depth {
public:
  enum Tp {
    One,
    Eight,
    Sixteen,
    ThirtyTwo
  };

private:
  template <Tp>
  struct UnderlyingS;

public:
  template <Tp value>
  using Underlying = typename UnderlyingS<value>::Tp;

  template <Tp Dh>
  static constexpr std::uint64_t Length = sizeof(Underlying<Dh>);

  template <Tp value>
  static constexpr Underlying<value> Max = std::numeric_limits<Underlying<value>>::max();

  template <Tp value>
  static constexpr Underlying<value> Min = 0;
};

template <>
struct Depth::UnderlyingS<Depth::One> {
  using Tp = std::uint8_t;
};

template <>
struct Depth::UnderlyingS<Depth::Eight> {
  using Tp = std::uint8_t;
};

template <>
struct Depth::UnderlyingS<Depth::Sixteen> {
  using Tp = std::uint16_t;
};

template <>
struct Depth::UnderlyingS<Depth::ThirtyTwo> {
  using Tp = std::uint32_t;
};
};
