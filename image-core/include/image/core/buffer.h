
#pragma once

#include <file/type.h>
#include <image/core/error.h>
#include <cstdint>
#include <array>
#include <vector>

namespace Image {

class BufferError : public Error {
protected:
  explicit BufferError(
    std::string_view message

  ) : Error(Namespace("BufferError", message)) {}
};
namespace BufferErrorMessage {
struct OutOfRange : BufferError {
  explicit OutOfRange(
    std::string_view message 

  ) : BufferError(Message("OutOfRange", message)) {}
};
};
class Buffer {
public:
  static constexpr std::uint64_t ChannelCount = 4;

  class Element {
  public:

    Element() = default;
    Element(
      std::uint8_t red,
      std::uint8_t green,
      std::uint8_t blue,
      std::uint8_t alpha = 0xff) 
    : data_{ 
      red, 
      green, 
      blue,
      alpha 
    } {}

    template <typename Self>
    static decltype(auto) SubscriptOperator(Self &self, std::uint64_t index) {
      if (index >= ChannelCount) {
        throw BufferErrorMessage::OutOfRange("Channel index out of range");
      }
      return self.data_[index];
    }
    std::uint8_t &operator[](std::uint64_t index) {
      return SubscriptOperator(*this, index);
    }
    const std::uint8_t &operator[](std::uint64_t index) const {
      return SubscriptOperator(*this, index);
    }

    std::uint8_t &Red() {
      return data_[0];
    }
    std::uint8_t Red() const {
      return data_[0];
    }
    std::uint8_t &Green() {
      return data_[1];
    }
    std::uint8_t Green() const {
      return data_[1];
    }
    std::uint8_t &Blue() {
      return data_[2];
    }
    std::uint8_t Blue() const {
      return data_[2];
    }
    std::uint8_t &Alpha() {
      return data_[3];
    }
    std::uint8_t Alpha() const {
      return data_[3];
    }

    auto begin() {
      return data_.begin();
    }
    auto begin() const {
      return data_.begin();
    }
    auto cbegin() const {
      return data_.cbegin();
    }
    auto end() {
      return data_.end();
    }
    auto end() const {
      return data_.end();
    }
    auto cend() const {
      return data_.cend();
    }

  private:
    std::array<std::uint8_t, 4> data_;
  };

  template <typename B>
  friend class SubscriptOperatorReturn;

  template <typename B>
  class SubscriptOperatorReturn {
  public:
    using UsedBuffer = B;

    explicit SubscriptOperatorReturn(
      UsedBuffer &buffer, std::uint64_t index

    ) : buffer_(buffer), index_(index) {}

    decltype(auto) operator=(const Element &other) {
      std::copy(
        other.begin(), other.end(), buffer_.data_.begin() + index_
      );
      return *this;
    }
    decltype(auto) operator=(Element &&other) { 
      std::move(
        other.begin(), other.end(), buffer_.data_.begin() + index_
      );
      return *this;
    }

    std::uint8_t &operator[](std::uint64_t channel) {
      return SubscriptOperator(*this, channel);
    }
    const std::uint8_t &operator[](std::uint64_t channel) const {
      return SubscriptOperator(*this, channel);
    }

    bool operator==(const SubscriptOperatorReturn &other) const {
      return std::equal(
        this->buffer_.data_.begin() + index_,
        this->buffer_.data_.begin() + index_ + ChannelCount,
        other.buffer_.data_.begin() + other.index_
      );
    }
    bool operator==(const Element &other) const {
      return std::equal(
        this->buffer_.data_.begin() + index_,
        this->buffer_.data_.begin() + index_ + ChannelCount,
        other.begin()
      );
    }
  private:
    UsedBuffer &buffer_; std::uint64_t index_;

    template <typename Self>
    static decltype(auto) SubscriptOperator(
      Self &self, std::uint64_t channel
    ) {
      return self.buffer_.data_[self.index_ + channel];
    }
  };
  template <typename B>
  class Iterator {
  public:
    using UsedBuffer = B;

    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = SubscriptOperatorReturn<B>;
    using pointer           = SubscriptOperatorReturn<B>*;
    using reference         = SubscriptOperatorReturn<B>&;

    explicit Iterator(
      UsedBuffer &buffer, std::uint64_t index

    ) : buffer_(buffer), index_(index) {}

    decltype(auto) operator*() {
      return buffer_[index_]; 
    }
    auto operator++() {
      index_++; return *this;
    }
    auto operator++(int) {
      auto temp = *this; operator++(); 

      return temp;
    }
    bool operator==(const Iterator &other) const {
      return index_ == other.index_;
    }
    bool operator!=(const Iterator &other) const {
      return !operator==(other);
    }
    decltype(auto) operator+(std::uint64_t value) {
      return Iterator(buffer_, index_ + value);
    }
  private:
    UsedBuffer &buffer_; std::uint64_t index_;
  };
  explicit Buffer() = default;
  explicit Buffer(
    std::uint64_t row_count, std::uint64_t column_count

  ) : row_count_(row_count), column_count_(column_count), data_((row_count * column_count) * ChannelCount) {}

  explicit Buffer(
    std::int32_t row_count,
    std::int32_t column_count,

    File::ByteSequence data
  
  ) : row_count_(row_count), column_count_(column_count), data_(std::move(data)) {}

  template <typename Self>
  static auto SubscriptOperator(Self &self, std::uint64_t index) {
    if (index >= self.Length()) {
      throw BufferErrorMessage::OutOfRange("Element index out of range");
    }
    return SubscriptOperatorReturn(self, index * ChannelCount);
  }
  auto operator[](std::uint64_t index) {
    return SubscriptOperator(*this, index);
  }
  auto operator[](std::uint64_t index) const {
    return SubscriptOperator(*this, index);
  }

  template <typename Self>
  static auto FunctionCallOperator(
    Self &self, std::uint64_t row, std::uint64_t column
  ) {
    if (row >= self.row_count_ || column >= self.column_count_) {
      throw BufferErrorMessage::OutOfRange("Element index out of range");
    }
    return self[row * self.column_count_ + column];
  }
  auto operator()(std::uint64_t row, std::uint64_t column) {
    return FunctionCallOperator(*this, row, column);
  }
  auto operator()(std::uint64_t row, std::uint64_t column) const {
    return FunctionCallOperator(*this, row, column);
  }

  std::uint64_t ByteCount() const {
    return data_.size();
  }
  std::uint64_t Length() const {
    return ByteCount() / ChannelCount;
  }

  auto begin() {
    return Iterator(*this, 0);
  }
  auto begin() const {
    return Iterator(*this, 0);
  }
  auto cbegin() const {
    return Iterator(*this, 0);
  }
  auto end() {
    return Iterator(*this, Length());
  }
  auto end() const {
    return Iterator(*this, Length());
  }
  auto cend() const {
    return Iterator(*this, Length());
  }

  std::uint64_t RowCount() const {
    return row_count_;
  }
  std::uint64_t ColumnCount() const {
    return column_count_;
  }
  File::ByteSpan Data() const {
    return data_;
  }

private:
  std::uint64_t row_count_    = 0;
  std::uint64_t column_count_ = 0;

  std::vector<std::uint8_t> data_;
};
};
