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
template <Depth::Tp depth = Depth::Eight, 
          Color::Tp color = Color::RGB>
class Buffer {
public:

  template <typename BufferTy>
  class Iterator;

  static constexpr Depth::Tp     depth_value   = depth;
  static constexpr Color::Tp     color_value   = color;
  static constexpr std::uint64_t channel_count = Color::ChannelCount<color>;
  static constexpr std::uint64_t ElementBCount = Element<depth, color>::BCount;

  Buffer() = default;

  Buffer(std::uint64_t row_count, std::uint64_t column_count, std::vector<Element<depth, color>> data) 
    : row_count_    (row_count) 
    , column_count_ (column_count) 
    , data_         (std::move(data)) {}

  Buffer(std::uint64_t row_count, std::uint64_t column_count, Depth::Underlying<depth> value) 
    : row_count_    (row_count)
    , column_count_ (column_count) 
    , data_         (std::vector<Element<depth, color>>(row_count_ * column_count_, Element<depth, color>(value))) {}

  Buffer(std::uint64_t row_count, std::uint64_t column_count) : Buffer(row_count, column_count, Depth::Min<depth>) {}

  bool operator==(const Buffer<depth, color> &other) const {
    return row_count_    == other.row_count_    && 
           column_count_ == other.column_count_ && 
           data_         == other.data_;
  }
  bool operator!=(const Buffer<depth, color> &other) const {
    return !operator==(other);
  }

  static Buffer<depth, color> From(
    std::array<Buffer<depth, Color::Grayscale>, Color::ChannelCount<color>> channel_list
  ) {
    std::uint64_t row_count    = channel_list[0].GetRCount();
    std::uint64_t column_count = channel_list[0].GetCCount();

    Buffer<depth, color> output(row_count, column_count);

    for (auto channel_index = 0u; 
              channel_index < Color::ChannelCount<color>; 
              channel_index++) {
      for (auto index = 0u; 
                index < output.GetLength();
                index++) {
        output[index][channel_index] = channel_list[channel_index][index];
      }
    }
    return output;
  }

  std::uint64_t GetRCount() const {
    return row_count_;
  }
  std::uint64_t GetCCount() const {
    return column_count_;
  }
  std::uint64_t GetLength() const {
    return data_.size();
  }
  std::uint64_t GetBCount() const {
    return GetLength() * Element<depth, color>::bcount;
  }
  bool IsEmpty() const {
    return GetLength();
  }

  Element<depth, color> &operator[](std::uint64_t index) {
    return data_[index];
  }
  const Element<depth, color> &operator[](std::uint64_t index) const {
    return data_[index];
  }
  Element<depth, color> &operator()(std::uint64_t row, std::uint64_t column) {
    return operator[](row * column_count_ + column);
  }
  const Element<depth, color> &operator()(std::uint64_t row, std::uint64_t column) const {
    return operator[](row * column_count_ + column);
  }

private:
  std::uint64_t row_count_;
  std::uint64_t column_count_;
  std::vector<Element<depth, color>> data_;

};

template <Depth::Tp depth, Color::Tp color>
template <typename BufferTy>
class Buffer<depth, color>::Iterator {
public:
  explicit Iterator(BufferTy &container, std::uint64_t index) 
    : container_(container) 
    , index_(index) {}

  std::conditional_t<!std::is_const_v<BufferTy>,
          Element<depth, color>,
    const Element<depth, color>> &
  operator*() const {
    return container_[index_];
  }

  Iterator<BufferTy> &operator++() {
    index_++;
    return *this;
  } 
  Iterator<BufferTy> operator++(int) {
    Iterator<BufferTy> temp(container_, index_);
    operator++();
    return temp;
  }

  bool operator==(const Iterator<BufferTy> &other) const {
    return index_ == other.index_;
  }
  bool operator!=(const Iterator<BufferTy> &other) const {
    return !operator==(other);
  }

private:
  BufferTy     &container_;
  std::uint64_t index_;
};

template <Depth::Tp depth, Color::Tp color> 
static typename Buffer<depth, color>::template Iterator<Buffer<depth, color>> begin(Buffer<depth, color> &buffer) {
  return typename Buffer<depth, color>::template Iterator<Buffer<depth, color>>(buffer, 0);
}
template <Depth::Tp depth, Color::Tp color> 
static typename Buffer<depth, color>::template Iterator<const Buffer<depth, color>> begin(const Buffer<depth, color> &buffer) {
  return typename Buffer<depth, color>::template Iterator<const Buffer<depth, color>>(buffer, 0);
}

template <Depth::Tp depth, Color::Tp color> 
static typename Buffer<depth, color>::template Iterator<Buffer<depth, color>> end(Buffer<depth, color> &buffer) {
  return typename Buffer<depth, color>::template Iterator<Buffer<depth, color>>(buffer, buffer.GetLength());
}
template <Depth::Tp depth, Color::Tp color> 
static typename Buffer<depth, color>::template Iterator<const Buffer<depth, color>> end(const Buffer<depth, color> &buffer) {
  return typename Buffer<depth, color>::template Iterator<const Buffer<depth, color>>(buffer, buffer.GetLength());
}

};
