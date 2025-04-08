#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdint>
#include <array>
#include <vector>

#include <image/core/color.h>
#include <image/core/depth.h>
#include <image/element.h>

namespace Image {

template <Depth::Tp DepthV  = DefDepth, 
          Color::Tp ColorV  = DefColor,
          bool AlphaSetting = DisableAlpha>
class Buffer {
public:
  static constexpr Depth::Tp     DepthValue   = DepthV;
  static constexpr Color::Tp     ColorValue   = ColorV;
  static constexpr bool          AlphaEnabled = AlphaSetting;
  static constexpr std::uint64_t ChannelCount = Color::ChannelCount<ColorV, AlphaSetting>;

  using UsedElement = Element<DepthV, ColorV, AlphaSetting>;

  static constexpr std::uint64_t ByteCountInElement = UsedElement::ByteCount;

  using Container = std::vector<UsedElement>;

  Buffer() = default;
  Buffer(std::uint64_t row_count, std::uint64_t column_count, Container data) 
    : row_count_    (row_count) 
    , column_count_ (column_count) 
    , data_         (std::move(data)) {}

  using Value = typename UsedElement::Value;

  Buffer(std::uint64_t row_count, std::uint64_t column_count, Value value) 
    : row_count_    (row_count)
    , column_count_ (column_count) 
    , data_         (Container(row_count_ * column_count_, UsedElement(value))) {}

  Buffer(std::uint64_t row_count, std::uint64_t column_count) 
    : Buffer(row_count, column_count, 0) {}

  bool operator==(const Buffer &other) const {
    return row_count_    == other.row_count_    && 
           column_count_ == other.column_count_ && 
           data_         == other.data_;
  }
  bool operator!=(const Buffer &other) const {
    return !operator==(other);
  }
  std::uint64_t GetRowCount() const {
    return row_count_;
  }
  std::uint64_t GetColumnCount() const {
    return column_count_;
  }
  std::uint64_t GetLength() const {
    return data_.size();
  }
  std::uint64_t GetByteCount() const {
    return GetLength() * ByteCountInElement;
  }
  bool IsEmpty() const {
    return GetLength() == 0;
  }

  UsedElement &operator[](std::uint64_t index) {
    return data_[index];
  }
  const UsedElement &operator[](std::uint64_t index) const {
    return data_[index];
  }
  UsedElement &operator()(std::uint64_t row, std::uint64_t column) {
    return operator[](row * column_count_ + column);
  }
  const UsedElement &operator()(std::uint64_t row, std::uint64_t column) const {
    return operator[](row * column_count_ + column);
  }

  using Iterator      = typename Container::iterator;
  using ConstIterator = typename Container::const_iterator;

  using RevIterator      = typename Container::reverse_iterator;
  using ConstRevIterator = typename Container::const_reverse_iterator;

  auto Begin() {
    return data_.begin();
  }
  auto Begin() const {
    return data_.begin();
  }
  auto ConstBegin() const { 
    return data_.cbegin();
  }
  auto End() {
    return data_.end();
  }
  auto End() const {
    return data_.end();
  }
  auto ConstEnd() const {
    return data_.cend();
  }
  auto RevBegin() {
    return data_.rbegin();
  }
  auto RevBegin() const {
    return data_.rbegin();
  }
  auto ConstRevBegin() const {
    return data_.crbegin();
  }
  auto RevEnd() {
    return data_.rend();
  }
  auto RevEnd() const {
    return data_.rend();
  }
  auto ConstRevEnd() const {
    return data_.crend();
  }

private:
  std::uint64_t row_count_;
  std::uint64_t column_count_;

  Container data_;
};

template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
auto begin(Buffer<DepthV, ColorV, AlphaSetting> &buffer) {
  return buffer.Begin();
}
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
auto begin(const Buffer<DepthV, ColorV, AlphaSetting> &buffer) {
  return buffer.Begin();
}
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
auto end(Buffer<DepthV, ColorV, AlphaSetting> &buffer) {
  return buffer.End();
}
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
auto end(const Buffer<DepthV, ColorV, AlphaSetting> &buffer) {
  return buffer.End();
}

template <Depth::Tp DepthV = DefDepth,
          Color::Tp ColorV = DefColor>
using AlphaBuffer = Buffer<DepthV, ColorV, EnableAlpha>;

};
