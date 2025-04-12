
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

public:
  using Iterator      = typename Container::iterator;
  using ConstIterator = typename Container::const_iterator;

  auto Begin() {
    return data_.begin();
  }
  auto Begin() const {
    return data_.begin();
  }
  auto ConstBegin() const {
    return data_.begin();
  }
  auto End() {
    return data_.end();
  }
  auto End() const {
    return data_.end();
  }
  auto ConstEnd() const {
    return data_.end();
  }

private:
  Container data_;
};

template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto begin(ChannelArray<DepthV, ColorV, AlphaSetting> &input) {
  return input.Begin();
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto begin(const ChannelArray<DepthV, ColorV, AlphaSetting> &input) {
  return input.Begin();
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto end(ChannelArray<DepthV, ColorV, AlphaSetting> &input) {
  return input.End();
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto end(const ChannelArray<DepthV, ColorV, AlphaSetting> &input) {
  return input.End();
}

}

