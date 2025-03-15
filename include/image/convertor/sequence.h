
#pragma once

#include <image/core/color.h>
#include <image/core/depth.h>
#include <image/core/endianness.h>

#include <image/element.h>
#include <image/buffer.h>
#include <image/convertor/depth.h>
#include <image/convertor/endianness.h>
#include <image/convertor/color.h>

#include <vector>
#include <type_traits>

namespace Image {

template <typename T>
class SequenceConvertor {
public:
  static_assert(false);

  SequenceConvertor(T) {}

  SequenceConvertor(T, T) {};

}; // SequenceConvertor<T>

template <Depth::Tp Dh, Color::Tp Cr>
class SequenceConvertor<Buffer<Dh, Cr>> {
public:

  using InputTp  = Buffer<Dh, Cr>;
  using AlphaTp  = Buffer<Dh, Color::Grayscale>;
  using OutputTp = std::vector<std::uint8_t>;

  SequenceConvertor(const InputTp &input) : input_(input) {}

  template <Endianness::Tp Es = Endianness::Determine()>
  OutputTp ToSequence() const {
    OutputTp output;
    output.reserve(input_.GetBCount());

    for (const auto &element : input_) {
      PushElement<Es>(output, element);
    }
    /* return std::move(output) */
    return output;
  }
  template <Endianness::Tp Es = Endianness::Determine()>
  OutputTp ToSequence(const AlphaTp &alpha) const {
    OutputTp output;  
    output.reserve(input_.GetBCount() + alpha.GetBCount());

    auto alpha_iterator = begin(alpha);

    for (const auto &element : input_) {
      PushElement<Es>(output,   element);
      PushChannel<Es>(output, **alpha_iterator++);
    }
    /*return std::move(output);*/
    return output;
  }

private:
  const InputTp &input_;

  template <Endianness::Tp Es>
  static void PushElement(OutputTp &output, const Element<Dh, Cr> &element) {
    if constexpr (Cr == Color::Grayscale) {
      PushChannel<Es>(output, *element);
    } else {
      for (auto channel : element) {
        PushChannel<Es>(output, channel);
      }
    }
  }
  template <Endianness::Tp Es>
  static void PushChannel(OutputTp &output, Depth::Underlying<Dh> channel) {
    if constexpr (Dh == Depth::Eight) {
      output.push_back(channel);
    } else {
      if constexpr (Endianness::Determine() == Es) {
        for (auto index = 0u; 
                  index < Depth::Length<Dh>;
                  index++) {
          output.push_back(reinterpret_cast<std::uint8_t *>(&channel)[index]);
        }
      } else {
        for (auto index = std::int64_t(Depth::Length<Dh> - 1);
                  index >= 0;
                  index--) {
          output.push_back(reinterpret_cast<std::uint8_t *>(&channel)[index]);
        }
      }
    }
  }
}; // SequenceConvertor<Buffer<depth, color>>

template <>
class SequenceConvertor<std::vector<std::uint8_t>::const_iterator> {
public:

  using IteratorTp = std::vector<std::uint8_t>::const_iterator;

  SequenceConvertor(IteratorTp begin, IteratorTp end) : begin_(begin), end_(end) {}

  template <Depth::Tp Dh, Color::Tp Cr, Endianness::Tp Es = Endianness::Determine()>
  Buffer<Dh, Cr> ToBuffer(std::uint32_t row_count, std::uint32_t column_count) const {
    Buffer<Dh, Cr> output(row_count, column_count);

    auto iterator = begin_;

    for (auto &element : output) {
      element = ElementFrom<Dh, Cr, Es>(
        iterator  - element.BCount,
        iterator += element.BCount
      );
    }
    return std::move(output);
  }

  template <Depth::Tp Dh, Color::Tp Cr, Endianness::Tp Es = Endianness::Determine()>
  Buffer<Dh, Cr> ToBuffer(std::uint32_t row_count, std::uint32_t column_count, Buffer<Dh, Color::Grayscale> &alpha) const {
    Buffer<Dh, Color::RGB> output(row_count, column_count);

    alpha = Buffer<Dh, Color::Grayscale>(row_count, column_count);

    auto iterator       = begin_;
    auto alpha_iterator = begin(alpha);

    for (auto &element : output) {
      element = ElementFrom<Dh, Cr, Es>(
        iterator  - element.BCount,
        iterator += element.BCount
      );
      **alpha_iterator++ = ChannelFrom<Dh, Es>(
        iterator  - alpha.ElementBCount,
        iterator += alpha.ElementBCount
      );
    }
    if constexpr (Cr == Color::RGB) {
      return std::move(output);
    } else {
      return ColorConvertor(output).template Convert<Cr>();
    }
  }
  template <Depth::Tp Dh, Color::Tp Cr, Endianness::Tp Es = Endianness::Determine()> 
  Buffer<Dh, Cr> ToBuffer(std::uint32_t row_count, std::uint32_t column_count, Depth::Tp depth, Color::Tp color) const {
    auto Convert = [](auto buffer) {
      return DepthConvertor(ColorConvertor(buffer).template Convert<Cr>()).template Convert<Dh>();
    };
    #define CONVERT(Dh, Cr) \
      if (depth == Dh && color == Cr) {\
        return Convert(ToBuffer<Dh, Cr, Es>(row_count, column_count)); \
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
  IteratorTp begin_;
  IteratorTp end_;

  template <Depth::Tp Dh, Endianness::Tp Es>
  static Depth::Underlying<Dh> ChannelFrom(IteratorTp iterator, IteratorTp end) {
    Depth::Underlying<Dh> output;
    
    if constexpr (Dh == Depth::Eight) {
      output = *iterator;
    } else {
      if constexpr (Endianness::Determine() == Es) {
        for (auto index = 0u; 
                  index < Depth::Length<Dh>;
                  index++) {
          reinterpret_cast<std::uint8_t *>(&output)[index] = *iterator++; 
        }
      } else {
        for (auto index = Depth::Length<Dh>;
                  index >= 0u;
                  index--) {
          reinterpret_cast<std::uint8_t *>(&output)[index] = *iterator++; 
        }
      }
    }
    return output;
  }

  template <Depth::Tp Dh, Color::Tp Cr, Endianness::Tp Es> 
  static Element<Dh, Cr> ElementFrom(IteratorTp iterator, IteratorTp end) {
    Element<Dh, Cr> output;

    if constexpr (Cr == Color::Grayscale) {
      *output = ChannelFrom<Dh, Es>(
        iterator  - Depth::Length<Dh>,
        iterator += Depth::Length<Dh>
      );
    } else {
      for (auto &channel : output) {
        channel = ChannelFrom<Dh, Es>(
          iterator  - Depth::Length<Dh>,
          iterator += Depth::Length<Dh>
        );
      }
    }
    return std::move(output);
  }
}; // SequenceConvertor<std::vector<std::uint8_t>::const_iterator>

template <>
class SequenceConvertor<std::vector<std::uint8_t>> {
public:

  using InputTp = std::vector<std::uint8_t>;

  SequenceConvertor(const InputTp &input) : input_(input) {}

  template <Depth::Tp Dh, Color::Tp Cr, Endianness::Tp Es = Endianness::Determine()> 
  Buffer<Dh, Cr> ToBuffer(std::uint32_t row_count, std::uint32_t column_count) const {
    return Image::SequenceConvertor(
      input_.cbegin(),
      input_.cend()
    ).template ToBuffer<Dh, Cr, Es>(row_count, column_count);
  }

  template <Depth::Tp Dh, Color::Tp Cr, Endianness::Tp Es = Endianness::Determine()> 
  Buffer<Dh, Cr> ToBuffer(std::uint32_t row_count, std::uint32_t column_count, Buffer<Dh, Color::Grayscale> &alpha) const {
    return Image::SequenceConvertor(
      input_.cbegin(),
      input_.cend()
    ).template ToBuffer<Dh, Cr, Es>(row_count, column_count, alpha);
  }

private:
  const InputTp &input_;

}; // SequenceConvertor<std::vector<std::uint8_t>>
}; // Image
