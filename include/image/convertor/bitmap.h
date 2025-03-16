
#pragma once

#include <image/core/depth.h>
#include <image/convertor/depth.h>

#include <vector>
#include <cstdint>
#include <bitset>

namespace Image {

class BitmapConvertor {
public:

  explicit BitmapConvertor(const std::vector<std::uint8_t> &input) : input_(input) {}

  std::vector<std::uint8_t> FromBitmap(std::uint64_t bit_count) const {
    std::vector<std::uint8_t> output;

    output.reserve(input_.size() * 8);

    auto current_bit = 0u;
    for (const auto &value : input_) {
      for (auto bit_index =  7;
                bit_index >= 0;
                bit_index--) {
        if (current_bit == bit_count) {
          break;
        }
        output.push_back(value & (1 << bit_index) ? 255 : 0);
        current_bit++;
      }
    }
    return output;
  }

  std::vector<std::uint8_t> ToBitmap() const {
    std::vector<std::uint8_t> output;

    output.reserve(input_.size() / 8);

    for (auto index = 0u;
              index < input_.size();) {
      std::uint8_t value = 0u;

      for (auto bit_index =  7;
                bit_index >= 0;
                bit_index--) {
        if (index < input_.size()) {
          value |= (input_[index] ? 1 : 0) << bit_index;
        }
        index++;
      }
      output.push_back(value);
    }
    return output;
  }

private:
  const std::vector<std::uint8_t> input_;

}; // BitmapConvertor

}; // Image

