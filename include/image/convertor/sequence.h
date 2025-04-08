
#pragma once

#include <image/core/color.h>
#include <image/core/depth.h>
#include <image/core/endianness.h>

#include <image/element.h>
#include <image/buffer.h>
#include <image/convertor/depth.h>
#include <image/convertor/endianness.h>
#include <image/convertor/color.h>
#include <image/convertor/data.h>

#include <vector>
#include <type_traits>

namespace Image {

using Sequence = std::vector<uint8_t>;

template <typename InputT>
class SequenceConvertor {
public:
  static_assert(false);

  SequenceConvertor(InputT)         {}
  SequenceConvertor(InputT, InputT) {}
};

template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
class SequenceConvertor<Buffer<DepthV, ColorV, AlphaSetting>> {
public:

  using UsedBuffer  = Buffer  <DepthV, ColorV, AlphaSetting>;
  using UsedElement = Element <DepthV, ColorV, AlphaSetting>;

  using Value = typename UsedElement::Value;

  SequenceConvertor(const UsedBuffer &input) : input_(input) {}

  template <Endianness::Tp EndiannessV = Endianness::Determine()>
  auto Convert() const {
    Sequence output;
    output.reserve(input_.GetByteCount());

    for (const auto &element : input_) {
      PushElement<EndiannessV>(output, element);
    }
    return output;
  }
private:
  const UsedBuffer &input_;

  template <Endianness::Tp EndiannessV>
  static void PushElement(Sequence &output, const UsedElement &element) {
    if constexpr (ColorV == Color::Grayscale) {
      PushChannel<EndiannessV>(output, static_cast<Value>(element));
    } else {
      for (auto channel : element) {
        PushChannel<EndiannessV>(output, channel);
      }
    }
  }
  template <Endianness::Tp EndiannessV>
  static void PushChannel(Sequence &output, Value channel) {
    if constexpr (DepthV == Depth::Eight) {
      output.push_back(channel);
    } else {
      if constexpr (Endianness::Determine() == EndiannessV) {
        for (auto index = 0u; 
                  index < Depth::Length<DepthV>;
                  index++) {
          output.push_back(reinterpret_cast<std::uint8_t *>(&channel)[index]);
        }
      } else {
        for (auto index = std::int64_t(Depth::Length<DepthV> - 1);
                  index >= 0;
                  index--) {
          output.push_back(reinterpret_cast<std::uint8_t *>(&channel)[index]);
        }
      }
    }
  }
};

template <>
class SequenceConvertor<Sequence::const_iterator> {
public:

  using Iterator = Sequence::const_iterator;

  SequenceConvertor(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

  template <Depth::Tp DepthV, 
            Color::Tp ColorV,
            bool AlphaSetting, 
            Endianness::Tp EndiannessV = Endianness::Determine()>
  auto Convert(
    std::uint32_t row_count, 
    std::uint32_t column_count
  ) const {
    Buffer<DepthV, ColorV, AlphaSetting> output(row_count, column_count);

    auto iterator = begin_;

    for (auto &element : output) {
      element = ElementFrom<DepthV, ColorV, AlphaSetting, EndiannessV>(
        iterator  - element.ByteCount,
        iterator += element.ByteCount
      );
    }
    return output;
  }
  template <Depth::Tp DepthV, 
            Color::Tp ColorV,
            bool AlphaSetting, 
            Endianness::Tp EndiannessV = Endianness::Determine()> 
  auto Convert(
    std::uint32_t row_count, 
    std::uint32_t column_count, 
    Depth::Tp depth, 
    Color::Tp color
  ) const {
    #define CONVERT(InputDepthV, InputColorV)                                         \
      if (depth == InputDepthV && color == InputColorV) {                             \
        return DataConvertor(Convert<                                                 \
          InputDepthV,                                                                \
          InputColorV,                                                                \
          AlphaSetting>(row_count, column_count)).template Convert<DepthV, ColorV>(); \
      }
    CONVERT(Depth::Eight     , Color::RGB);
    CONVERT(Depth::Sixteen   , Color::RGB);
    CONVERT(Depth::ThirtyTwo , Color::RGB);
    CONVERT(Depth::Eight     , Color::CMYK);
    CONVERT(Depth::Sixteen   , Color::CMYK);
    CONVERT(Depth::ThirtyTwo , Color::CMYK);
    CONVERT(Depth::Eight     , Color::Grayscale);
    CONVERT(Depth::Sixteen   , Color::Grayscale);
    CONVERT(Depth::ThirtyTwo , Color::Grayscale);
    throw std::runtime_error("Failure");

    #undef CONVERT
  }

public:
  Iterator begin_;
  Iterator end_;

  template <Depth::Tp DepthV, Endianness::Tp EndiannessV>
  static auto ChannelFrom(Iterator iterator, Iterator end) {
    Depth::Underlying<DepthV> output;
    
    if constexpr (DepthV == Depth::Eight) {
      output = *iterator;
    } else {
      if constexpr (Endianness::Determine() == EndiannessV) {
        for (auto index = 0u; 
                  index < Depth::Length<DepthV>;
                  index++) {
          reinterpret_cast<std::uint8_t *>(&output)[index] = *iterator++; 
        }
      } else {
        for (auto index = Depth::Length<DepthV>;
                  index >= 0u;
                  index--) {
          reinterpret_cast<std::uint8_t *>(&output)[index] = *iterator++; 
        }
      }
    }
    return output;
  }
  template <Depth::Tp DepthV, Color::Tp ColorV, bool AlphaSetting, Endianness::Tp EndiannessV> 
  static auto ElementFrom(Iterator iterator, Iterator end) {
    Element<DepthV, ColorV, AlphaSetting> output;

    for (decltype(auto) channel : output) {
      channel = ChannelFrom<DepthV, EndiannessV>(
        iterator  - Depth::Length<DepthV>,
        iterator += Depth::Length<DepthV>
      );
    }
    return output;
  }
};
template <>
class SequenceConvertor<Sequence> {
public:
  SequenceConvertor(const Sequence &input) : input_(input) {}

  template <Depth::Tp DepthV, 
            Color::Tp ColorV,
            bool AlphaSetting,
            Endianness::Tp EndiannessV = Endianness::Determine()> 
  auto Convert(
    std::uint32_t row_count, 
    std::uint32_t column_count
  ) const {
    return Image::SequenceConvertor(
      input_.cbegin (),
      input_.cend   ()
    ).template Convert<DepthV, ColorV, AlphaSetting, EndiannessV>(row_count, column_count);
  }
private:
  const Sequence &input_;
}; 
};
