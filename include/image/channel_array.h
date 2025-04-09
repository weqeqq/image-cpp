
#pragma once

#include <image/buffer.h>

namespace Image {

template <Depth::Tp DepthV  = DefDepth,
          Color::Tp ColorV  = DefColor,
          bool AlphaSetting = DisableAlpha>
class ChannelArray {
public:
  using UsedBuffer = Buffer<DepthV, Color::Grayscale>;

  static constexpr std::uint64_t ChannelCount = Color::ChannelCount<ColorV, AlphaSetting>;

  ChannelArray() = default;
  ChannelArray(std::uint64_t row_count, std::uint64_t column_count) {

    for (decltype(auto) channel : data_) {
      channel = UsedBuffer(row_count, column_count);
    }
  }
  UsedBuffer &operator[](std::uint64_t index) {
    return data_[index];
  }
  const UsedBuffer &operator[](std::uint64_t index) const {
    return data_[index];
  }

  std::uint64_t GetRowCount() const {
    return (*this)[0].GetRowCount();
  }
  std::uint64_t GetColumnCount() const {
    return (*this)[0].GetColumnCount();
  }
  std::uint64_t GetLength() const {
    return (*this)[0].GetLength();
  }

private:
  using Container = std::array<UsedBuffer, ChannelCount>;
  Container data_;
};
};

