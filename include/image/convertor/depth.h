
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
};
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
class DepthConvertor<Element<DepthV, ColorV, AlphaSetting>> {
public:
  template <Depth::Tp CustomDepthV>
  using CustomElement = Element<CustomDepthV, ColorV, AlphaSetting>;

  using UsedElement = CustomElement<DepthV>;

  template <Depth::Tp CustomDepthV>
  using CustomValue = typename CustomElement<CustomDepthV>::Value;

  using Value = CustomValue<DepthV>;

  template <Depth::Tp CustomDepthV>
  static constexpr CustomValue<CustomDepthV> CustomWhite = Depth::Max<CustomDepthV>;

  static constexpr Value White = CustomWhite<DepthV>;

  template <Depth::Tp CustomDepthV>
  static constexpr CustomValue<CustomDepthV> CustomBlack = Depth::Min<CustomDepthV>;

  static constexpr Value Black = CustomBlack<DepthV>;

  DepthConvertor(const UsedElement &input) : input_(input) {}

  template <Depth::Tp OutputDepthV>
  decltype(auto) Convert() const {
    if constexpr (DepthV == OutputDepthV) {
      return input_;
    } else {
      CustomElement<OutputDepthV> output;
      for (auto index = 0u;
                index < Color::ChannelCount<ColorV, DisableAlpha>;
                index++) {
        output[index] = static_cast<CustomValue<OutputDepthV>>(
          (static_cast<double>(input_[index]) / White) * CustomWhite<OutputDepthV>
        );
      }
      if constexpr (UsedElement::AlphaEnabled) {
        output[Color::AlphaIndex<ColorV>] = input_[Color::AlphaIndex<ColorV>];
      }
      return output;
    }
  }
private:
  const UsedElement &input_;
};
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
class DepthConvertor<Buffer<DepthV, ColorV, AlphaSetting>> {
public:
  template <Depth::Tp CustomDepthV>
  using CustomBuffer = Buffer<CustomDepthV, ColorV, AlphaSetting>;

  using UsedBuffer = CustomBuffer<DepthV>;

  DepthConvertor(const UsedBuffer &input) : input_(input) {}

  template <Depth::Tp OutputDepthV>
  decltype(auto) Convert() const {
    if constexpr (DepthV == OutputDepthV) {
      return input_;
    } else {
      CustomBuffer<OutputDepthV> output(
        input_.GetRowCount    (), 
        input_.GetColumnCount ()
      );
      for (auto index = 0u;
                index < output.GetLength();
                index++) {
        output[index] = Image::DepthConvertor(input_[index]).template Convert<OutputDepthV>();
      }
      return output;
    }
  }
private:
  const UsedBuffer &input_;
}; 
};
