
#pragma once

#include <image/core/depth.h>
#include <image/core/color.h>

#include <image/buffer.h>
#include <image/element.h>

#include <algorithm>

namespace Image {

template <typename T>
class ColorConvertor {
public:

  static_assert(false);

  ColorConvertor(T) {}

}; // ColorConvertor<T>

template <Depth::Tp Dh, Color::Tp Cr>
class ColorConvertor<Element<Dh, Cr>> {
private:
  class Gray;
  class CMYK;

public:
  ColorConvertor(const Element<Dh, Cr> &input) : input_(input) {}

  template <Color::Tp OCr> 
  std::conditional_t<Cr != OCr,
          Element<Dh, OCr>,
    const Element<Dh, OCr> &> 
  Convert() {
    if constexpr (Cr == OCr) {
      return input_;
    } else {
      return ConvertFromRGB<OCr>(ConvertToRGB(input_));
    }
  }

private:
  const Element<Dh, Cr> &input_;

  static std::conditional_t<Cr != Color::RGB, 
          Element<Dh, Color::RGB>,
    const Element<Dh, Color::RGB> &>
  ConvertToRGB(const Element<Dh, Cr> &input);

  template <Color::Tp output_color>
  static std::conditional_t<output_color != Color::RGB,
          Element<Dh, output_color>,
    const Element<Dh, output_color> &>
  ConvertFromRGB(const Element<Dh, Color::RGB> &input);

}; // ColorConvertor<Element<Dh, Cr>>

template <Depth::Tp Dh, Color::Tp Cr>
class ColorConvertor<Element<Dh, Cr>>::Gray {
public:
  static Element<Dh, Color::RGB> ConvertToRGB(const Element<Dh, Color::Grayscale> &input) {
    Element<Dh, Color::RGB> output;
    output[0] = input[0];
    output[1] = input[0];
    output[2] = input[0];
    return std::move(output);
  }
  static Element<Dh, Color::Grayscale> ConvertFromRGB(const Element<Dh, Color::RGB> &input) {
    Element<Dh, Color::Grayscale> output;
    output[0] = 0.299 * input[0] + 
                0.587 * input[1] + 
                0.114 * input[2];
    return std::move(output);
  }
}; // ColorConvertor<Element<Dh, Cr>>::Gray

template <Depth::Tp Dh, Color::Tp Cr> 
class ColorConvertor<Element<Dh, Cr>>::CMYK {
public:
  static Element<Dh, Color::RGB> ConvertToRGB(const Element<Dh, Color::CMYK> &input) {
    Element<Dh, Color::RGB> output;
    output[0] = ConvertChannelToRGB(input[0], input[3]);
    output[1] = ConvertChannelToRGB(input[1], input[3]);
    output[2] = ConvertChannelToRGB(input[2], input[3]);
    return std::move(output);
  }
  static Element<Dh, Color::CMYK> ConvertFromRGB(const Element<Dh, Color::RGB> &input) {
    Depth::Underlying<Dh> black = Depth::Max<Dh> - std::max<Depth::Underlying<Dh>>({
      input[0],
      input[1],
      input[2]
    });
    Element<Dh, Color::CMYK> output;
    if (black == Depth::Max<Dh>) {
      output[0] = 0;
      output[1] = 0;
      output[2] = 0;
    } else {
      output[0] = ConvertChannelFromRGB(input[0], black);
      output[1] = ConvertChannelFromRGB(input[1], black);
      output[2] = ConvertChannelFromRGB(input[2], black);
    }
    output[3] = black;
    return std::move(output);
  }

private:
  static Depth::Underlying<Dh> ConvertChannelToRGB(Depth::Underlying<Dh> channel, Depth::Underlying<Dh> black) {
    return (Depth::Max<Dh> - channel) * (Depth::Max<Dh> - black) / Depth::Max<Dh>;
  }
  static Depth::Underlying<Dh> ConvertChannelFromRGB(Depth::Underlying<Dh> channel, Depth::Underlying<Dh> black) {
    return (Depth::Max<Dh> - channel - black) * Depth::Max<Dh> / (Depth::Max<Dh> - black);
  }
}; // ColorConvertor<Element<Dh, Cr>>::CMYK 

template <Depth::Tp Dh, Color::Tp Cr>
std::conditional_t<Cr != Color::RGB, 
        Element<Dh, Color::RGB>,
  const Element<Dh, Color::RGB> &>
ColorConvertor<Element<Dh, Cr>>::ConvertToRGB(const Element<Dh, Cr> &input) {
  if constexpr (Cr == Color::RGB) {
    return input;
  } else if constexpr (Cr == Color::Grayscale) {
    return Gray::ConvertToRGB(input);
  } else if constexpr (Cr == Color::CMYK) {
    return CMYK::ConvertToRGB(input);
  } else {
    static_assert(false);
  }
}

template <Depth::Tp Dh, Color::Tp Cr>
template <Color::Tp OCr>
std::conditional_t<OCr != Color::RGB,
        Element<Dh, OCr>,
  const Element<Dh, OCr> &>
ColorConvertor<Element<Dh, Cr>>::ConvertFromRGB(const Element<Dh, Color::RGB> &input) {
  if constexpr (OCr == Color::RGB) {
    return input;
  } else if constexpr (OCr == Color::Grayscale) {
    return Gray::ConvertFromRGB(input);
  } else if constexpr (OCr == Color::CMYK) {
    return CMYK::ConvertFromRGB(input);
  } else {
    static_assert(false);
  }
}

template <Depth::Tp Dh, Color::Tp Cr>
class ColorConvertor<Buffer<Dh, Cr>> {
public:
  ColorConvertor(const Buffer<Dh, Cr> &input) : input_(input) {}

  template <Color::Tp OCr> 
  std::conditional_t<Cr != OCr,
          Buffer<Dh, OCr>,
    const Buffer<Dh, OCr> &>
  Convert() {
    if constexpr (Cr == OCr) {
      return input_;
    } else {
      Buffer<Dh, OCr> output(input_.GetRCount(), input_.GetCCount());
      auto iterator = begin(input_);
      for (auto &element : output) {
        element = Image::ColorConvertor(*iterator++).template Convert<OCr>();
      }
      return std::move(output);
    }
  }

private:
  const Buffer<Dh, Cr> &input_;

}; // ColorConvertor<Buffer<Dh, Cr>>
}; // Image
