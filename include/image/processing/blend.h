
#pragma once

#include <image/buffer.h>
#include <image/convertor/color.h>
#include <random>

namespace Image {

class Blending {
public:
  enum Tp {
    PassThrough,
    Normal,
    Dissolve,
    Darken,
    Multiply,
    ColorBurn,
    LinearBurn,
    DarkerColor,
    Lighten,
    Screen,
    ColorDodge,
    LinearDodge,
    LighterColor,
    Overlay,
    SoftLight,
    HardLight,
    VividLight,
    LinearLight,
    PinLight,
    HardMix,
    Difference,
    Exclusion,
    Subtract,
    Divide,
    Hue,
    Saturation,
    Color,
    Luminosity,
  };
};

namespace Processing {
  
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
class BlendClass {
public:

  using UsedBuffer  = Buffer  <DepthV, ColorV, AlphaSetting>;
  using UsedElement = Element <DepthV, ColorV, AlphaSetting>;

  using Value = typename UsedElement::Value;

  static constexpr Value Max = Depth::Max<DepthV>;
  static constexpr Value Min = Depth::Min<DepthV>;

  UsedBuffer &background; const UsedBuffer &foreground;

  std::uint64_t x_offset = 0;
  std::uint64_t y_offset = 0;

  Blending::Tp blending = Blending::Normal;

  decltype(auto) Process() const {
    for (auto bg_row = y_offset; 
              bg_row < y_offset + foreground.GetRowCount();
              bg_row++) {
      for (auto bg_column = x_offset; 
                bg_column < x_offset + foreground.GetColumnCount();
                bg_column++) {

        auto fg_row    = bg_row    - y_offset;
        auto fg_column = bg_column - x_offset;

        decltype(auto) background_element = background(bg_row, bg_column);
        decltype(auto) foreground_element = foreground(fg_row, fg_column);

        auto tmp = background_element;

        BlendAlpha(background_element, BlendElement(
            tmp,
            foreground_element,
            blending
        ));
      }
    }
    return background;
  }

private:

  decltype(auto) BlendAlpha(
    UsedElement &background, const UsedElement &foreground
  ) const {
    if constexpr (AlphaSetting == EnableAlpha) {
      float alpha = static_cast<float>(foreground[Color::AlphaIndex<ColorV>]) / Max;
      for (auto index = 0u;
                index < Color::ChannelCount<ColorV, DisableAlpha>;
                index++) {
        background[index] = 
          (background[index] * (1.0f - alpha)) + 
          (foreground[index] * (       alpha));
      }
      background[Color::AlphaIndex<ColorV>] = std::max(
        background[Color::AlphaIndex<ColorV>],
        foreground[Color::AlphaIndex<ColorV>]
      );
    }
    return background;
  }

  decltype(auto) BlendNormal(
    UsedElement &background, const UsedElement &foreground
  ) const {
    background = foreground;
    return background;
  }

  decltype(auto) BlendDissolve(
    UsedElement &background, const UsedElement &foreground
  ) const {
    static bool initialized = false;

    if (!initialized) {
      std::srand(std::time(0)); initialized = true;
    }
    auto random = static_cast<Value>(
      (static_cast<double>(std::rand()) / RAND_MAX) * Max
    );
    if constexpr (AlphaSetting == EnableAlpha) {

      if (random < foreground[Color::AlphaIndex<ColorV>]) {
        background = foreground;
      }
    }
    return background;
  }

  decltype(auto) BlendDarken(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = std::min(background[index], foreground[index]);
    }
    return background;
  }

  decltype(auto) BlendMultiply(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = background[index] * foreground[index] / Max;
    }
    return background;
  }

  decltype(auto) BlendColorBurn(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      if (!foreground[index]) {
        background[index] = 0;
        continue;
      } 
      auto temp = static_cast<std::uint64_t>(Max - background[index]) * Max / foreground[index];
      background[index] = (temp > Max) 
         ? 0 
         : Max - temp;
    }
    return background;
  }

  decltype(auto) BlendLinearBurn(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = std::max<std::int64_t>(
        0l,
        static_cast<std::int64_t>(background[index]) + 
        static_cast<std::int64_t>(foreground[index]) - Max);
    }
    return background;
  }

  decltype(auto) BlendDarkerColor(
    UsedElement &background, const UsedElement &foreground
  ) const {
    auto CalculateLuminance = [](const auto &color) {
      auto Normalize = [](const auto &value) {
        return static_cast<float>(value) / Depth::Max<DepthV>;
      };
      auto rgb = ColorConvertor(color).template Convert<Color::RGB>();
      return 0.299f * Normalize(rgb[0]) + 
             0.587f * Normalize(rgb[1]) +
             0.114f * Normalize(rgb[2]);
    };
    if (CalculateLuminance(background) > CalculateLuminance(foreground)) {
      background = foreground;
    }
    return background;
  }

  decltype(auto) BlendLighten(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = std::max(background[index], foreground[index]);
    }
    return background;
  }

  decltype(auto) BlendScreen(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = Max - ((Max - background[index]) * (Max - foreground[index])) / Max;
    }
    return background;
  }

  decltype(auto) BlendColorDodge(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = (foreground[index] == Max)
        ? Max
        : std::min<std::uint64_t>(Max, (background[index] * Max) / (Max - foreground[index]));
    }
    return background;
  } 

  decltype(auto) BlendLinearDodge(
    UsedElement &background, const UsedElement &foreground
  ) const {
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = std::min<std::uint64_t>(
        Max, 
        static_cast<std::uint64_t>(background[index]) +
        static_cast<std::uint64_t>(foreground[index])
      );
    }
    return background;
  }

  decltype(auto) BlendLighterColor(
    UsedElement &background, const UsedElement &foreground
  ) const {
    auto CalculateLuminance = [](const auto &color) {
      auto Normalize = [](const auto &value) {
        return static_cast<float>(value) / Depth::Max<DepthV>;
      };
      auto rgb = ColorConvertor(color).template Convert<Color::RGB>();
      return 0.299f * Normalize(rgb[0]) + 
             0.587f * Normalize(rgb[1]) +
             0.114f * Normalize(rgb[2]);
    };
    if (CalculateLuminance(background) < CalculateLuminance(foreground)) {
      background = foreground;
    }
    return background;
  }
  decltype(auto) BlendOverlay(
    UsedElement &background, const UsedElement &foreground
  ) const {

    auto Blend = [](Value background, Value foreground) {
      float background_f = static_cast<float>(background) / Max;
      float foreground_f = static_cast<float>(foreground) / Max;
      float output;

      if (background_f < 0.5f) {
        output = 2.0f * background_f * foreground_f;
      } else {
        output = 1.0f - 2.0f * (1.0f - background_f) * (1.0f - foreground_f);
      }
      output = (output < 0.0f)
        ? 0.0f 
        : (output > 1.0f) 
          ? 1.0f 
          : output;

      return static_cast<Value>(output * Max + 0.5f);
    };

    for (auto index = 0u;
              index < Color::ChannelCount<ColorV, DisableAlpha>;
              index++) {
      background[index] = Blend(background[index], foreground[index]);
    }
    return background;
  }

  decltype(auto) BlendElement(
    UsedElement &background, const UsedElement &foreground, Blending::Tp blending
  ) const {
    switch (blending) {
      case Blending::Normal       : return BlendNormal       (background, foreground);
      case Blending::Dissolve     : return BlendDissolve     (background, foreground);
      case Blending::Darken       : return BlendDarken       (background, foreground);
      case Blending::Multiply     : return BlendMultiply     (background, foreground);
      case Blending::ColorBurn    : return BlendColorBurn    (background, foreground);
      case Blending::LinearBurn   : return BlendLinearBurn   (background, foreground);
      case Blending::DarkerColor  : return BlendDarkerColor  (background, foreground);
      case Blending::Lighten      : return BlendLighten      (background, foreground);
      case Blending::Screen       : return BlendScreen       (background, foreground);
      case Blending::ColorDodge   : return BlendColorDodge   (background, foreground);
      case Blending::LinearDodge  : return BlendLinearDodge  (background, foreground);
      case Blending::LighterColor : return BlendLighterColor (background, foreground);
      case Blending::Overlay      : return BlendOverlay      (background, foreground);
      default: {
        throw std::runtime_error("blerr");
      }
    }
  }
};
}; 
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
decltype(auto) Blend(
        Buffer<DepthV, ColorV, AlphaSetting> &buffer_a, 
  const Buffer<DepthV, ColorV, AlphaSetting> &buffer_b,
  std::uint64_t x_offset,
  std::uint64_t y_offset,
  Blending::Tp  blending
) {
  return Processing::BlendClass<DepthV, ColorV, AlphaSetting>({
    buffer_a, 
    buffer_b,
    x_offset, 
    y_offset, 
    blending
  }).Process();
}
};
