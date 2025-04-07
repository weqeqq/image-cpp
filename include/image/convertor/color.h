
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
}; 
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
class ColorConvertor<Element<DepthV, ColorV, AlphaSetting>> {
private:
  using UsedElement = Element<DepthV, ColorV, AlphaSetting>;

  template <Color::Tp CustomColorV>
  using CustomElement = Element<DepthV, CustomColorV, AlphaSetting>;

  using Value = typename UsedElement::Value;

  static constexpr Value Black = Depth::Min<DepthV>;
  static constexpr Value White = Depth::Max<DepthV>;

  class Grayscale {
  public:
    static auto ConvertToRGB(const CustomElement<Color::Grayscale> &input) {
      CustomElement<Color::RGB> output;

      output[0] = input[0];
      output[1] = input[0];
      output[2] = input[0];

      if constexpr (UsedElement::AlphaEnabled) {
        output[3] = input[1];
      }
      return output;
    }
    static auto ConvertFromRGB(const CustomElement<Color::RGB> &input) {
      CustomElement<Color::Grayscale> output;

      output[0] = 0.299 * input[0] + 
                  0.587 * input[1] + 
                  0.114 * input[2];

      if constexpr (UsedElement::AlphaEnabled) {
        output[1] = input[3];
      }
      return output;
    }
  };
  class CMYK {
  public:
    static auto ConvertToRGB(const CustomElement<Color::CMYK> &input) {
      CustomElement<Color::RGB> output;

      output[0] = ConvertChannelToRGB(input[0], input[3]);
      output[1] = ConvertChannelToRGB(input[1], input[3]);
      output[2] = ConvertChannelToRGB(input[2], input[3]);

      if constexpr (UsedElement::AlphaEnabled) {
        output[3] = input[4];
      }
      return output;
    }
    static auto ConvertFromRGB(const CustomElement<Color::RGB> &input) {
      Value black = White - std::max<Value>({
        input[0],
        input[1],
        input[2]
      });
      CustomElement<Color::CMYK> output;
      if (black == White) {
        output[0] = Black;
        output[1] = Black;
        output[2] = Black;
      } else {
        output[0] = ConvertChannelFromRGB(input[0], black);
        output[1] = ConvertChannelFromRGB(input[1], black);
        output[2] = ConvertChannelFromRGB(input[2], black);
      }
      output[3] = black;

      if constexpr (UsedElement::AlphaEnabled) {
        output[4] = input[3];
      }
      return output;
    }
  private:
    static Value ConvertChannelToRGB(Value channel, Value black) {
      return (White - channel) * (White - black) / White;
    }
    static Value ConvertChannelFromRGB(Value channel, Value black) {
      return (White - channel - black) * White / (White - black);
    }
  };
public:
  ColorConvertor(const UsedElement &input) : input_(input) {}

  template <Color::Tp OutputColorV>
  constexpr bool ConversionNeeded() {
    return ColorV != OutputColorV;
  }
  template <Color::Tp OutputColorV> 
  decltype(auto) Convert() {
    if constexpr (ColorV == OutputColorV) {
      return input_;
    } else {
      return ConvertFromRGB<OutputColorV>(ConvertToRGB(input_));
    }
  }
private:
  const UsedElement &input_;

  decltype(auto) ConvertToRGB(const UsedElement &input) {
    if constexpr (ColorV == Color::RGB) {
      return input;
    } else if constexpr (ColorV == Color::Grayscale) {
      return Grayscale::ConvertToRGB(input);
    } else if constexpr (ColorV == Color::CMYK) {
      return CMYK::ConvertToRGB(input);
    } else {
      static_assert(false);
    }
  }
  template <Color::Tp OutputColorV>
  decltype(auto) ConvertFromRGB(const CustomElement<Color::RGB> &input) {
    if constexpr (OutputColorV == Color::RGB) {
      return input_;
    } else if constexpr (OutputColorV == Color::Grayscale) {
      return Grayscale::ConvertFromRGB(input);
    } else if constexpr (OutputColorV == Color::CMYK) {
      return CMYK::ConvertFromRGB(input);
    } else {
      static_assert(false);
    }
  }
};
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
class ColorConvertor<Buffer<DepthV, ColorV, AlphaSetting>> {
public:
  template <Color::Tp CustomColorV>
  using CustomBuffer = Buffer<DepthV, CustomColorV, AlphaSetting>;

  using UsedBuffer = CustomBuffer<ColorV>;

  ColorConvertor(const UsedBuffer &input) : input_(input) {}

  template <Color::Tp OutputColorV> 
  decltype(auto) Convert() {
    if constexpr (ColorV == OutputColorV) {
      return input_;
    } else {
      CustomBuffer<OutputColorV> output(
        input_.GetRowCount    (), 
        input_.GetColumnCount ()
      );
      for (auto index = 0u;
                index < output.GetLength();
                index++) {
        output[index] = Image::ColorConvertor(input_[index]).template Convert<OutputColorV>();
      }
      return output;
    }
  }
private:
  const UsedBuffer &input_;
};
};
