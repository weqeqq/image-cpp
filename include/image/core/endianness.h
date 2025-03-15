
#pragma once

namespace Image {

class Endianness {
public:
  enum Tp {
    Little,
    Big,
    PDP,
  };

  static constexpr Tp Determine() {
#if   defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ 
      return Little;
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
      return Big;
#endif
  }

}; // Endianness
}; // Image
