
#pragma once

#include <image/core/color.h>
#include <image/core/depth.h>

#include <image/buffer.h>
#include <image/element.h>

namespace Image {

template <typename T>
class DepthConvertor {
public:

  static_assert(false);

  DepthConvertor(T) {}

}; // DepthConvertor<T>

template <Depth::Tp Dh, Color::Tp Cr>
class DepthConvertor<Element<Dh, Cr>> {
public:
  DepthConvertor(const Element<Dh, Cr> &input) : input_(input) {}

  template <Depth::Tp ODh>
  std::conditional_t<Dh != ODh,
          Element<ODh, Cr>,
    const Element<ODh, Cr> &>
  Convert() const {
    if constexpr (Dh == ODh) {
      return input_;
    } else {
      Element<ODh, Cr> output;
      auto output_iterator = begin(output);
      for (const auto &channel : input_) {
        *output_iterator++ = static_cast<Depth::Underlying<ODh>>(
          (static_cast<double>(channel) / Depth::Max<Dh>) * Depth::Max<ODh>
        );
      }
      return std::move(output);
    }
  }
private:
  const Element<Dh, Cr> &input_;

}; // DepthConvertor<Element<Dh, Cr>>

template <Depth::Tp Dh, Color::Tp Cr>
class DepthConvertor<Buffer<Dh, Cr>> {
public:
  DepthConvertor(const Buffer<Dh, Cr> &input) : input_(input) {}

  template <Depth::Tp ODh>
  std::conditional_t<Dh != ODh,
          Buffer<ODh, Cr>,
    const Buffer<ODh, Cr> &>
  Convert() const {
    if constexpr (Dh == ODh) {
      return input_;
    } else {
      Buffer<ODh, Cr> output(input_.GetRCount(), input_.GetCCount());
      auto iterator = begin(input_);
      for (auto &element : output) {
        element = Image::DepthConvertor(*iterator++).template Convert<ODh>();
      }
      return std::move(output);
    }
  }
private:
  const Buffer<Dh, Cr> &input_;

}; // DepthConvertor<Buffer<depth, color>>
}; // Image
