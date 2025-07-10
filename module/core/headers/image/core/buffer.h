
#pragma once

#include <cstdint>
#include <vector>

namespace Image {
using I8  = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int32_t;
using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using F32 = float;
using F64 = double;

template <typename T = U8>
class Buffer {
  std::vector<T> CreateBuffer() const {
    return std::vector<T>(
      row_count_    *
      column_count_ *
      channel_count_
    );
  }
public:
  Buffer(std::vector<T> buffer, unsigned row_count, unsigned column_count, unsigned channel_count = 4)
    : row_count_     ( row_count      )
    , column_count_  ( column_count   )
    , channel_count_ ( channel_count  )
    , buffer_        ( buffer         ) {}

  Buffer(unsigned row_count, unsigned column_count, unsigned channel_count = 4)
    : Buffer(std::vector<T>(
        row_count    *
        column_count *
        channel_count), row_count, column_count, channel_count) {}

  Buffer() : Buffer(0, 0) {}

  auto begin()       { return buffer_.begin(); }
  auto begin() const { return buffer_.begin(); }
  auto end()         { return buffer_.end();   }
  auto end() const   { return buffer_.end();   }

  unsigned RowCount()     const { return row_count_;                                  }
  unsigned ColumnCount()  const { return column_count_;                               }
  unsigned ChannelCount() const { return channel_count_;                              }
  unsigned Length()       const { return RowCount() * ColumnCount();                  }
  unsigned Count()        const { return RowCount() * ColumnCount() * ChannelCount(); }

  const std::vector<T> &Underlying() const {
    return buffer_;
  }
  T *Data() {
    return buffer_.data();
  }
  const T *Data() const {
    return buffer_.data();
  }

  bool operator==(const Buffer &other) const {
    return buffer_ == other.buffer_;
  }
  bool operator!=(const Buffer &other) const {
    return !operator==(other);
  }
  auto operator[](unsigned index) {
    return SubscriptOperator<decltype(this)>(this, index);
  }
  auto operator[](unsigned index) const {
    return SubscriptOperator<decltype(this)>(this, index);
  }
  auto operator()(unsigned row, unsigned column) {
    return SubscriptOperator<decltype(this)>(this, row, column);
  }
  auto operator()(unsigned row, unsigned column) const {
    return SubscriptOperator<decltype(this)>(this, row, column);
  }
private:
  unsigned row_count_     = 0;
  unsigned column_count_  = 0;
  unsigned channel_count_ = 0;
  std::vector<T> buffer_;

  template <typename S>
  static auto SubscriptOperator(S self, unsigned index) {
    return self->buffer_.data() + index * self->channel_count_;
  }
  template <typename S>
  static auto SubscriptOperator(S self, unsigned row, unsigned column) {
    return SubscriptOperator<S>(
      self,
      row * self->column_count_ + column
    );
  }
};
};
