
#pragma once

#include <image/buffer.h>
#include <image/convertor/color.h>
#include <random>

namespace Image {

class Blending {
public:
  enum Tp {
    Normal,
    Dissolve,
    Darken,
    Multiply,
    ColorBurn,
    LinearBurn,
    DarkerColor,
    Lighten,
  };
};

namespace Processing {
  
template <Depth::Tp DepthV, Color::Tp ColorV>
class BlendClass {
public:

  using ElementTp      = Element<DepthV, ColorV>;
  using AlphaElementTp = Element<DepthV, Color::Grayscale>;
  using BufferTp       = Buffer<DepthV, ColorV>;
  using AlphaTp        = Buffer<DepthV, Color::Grayscale>;

  struct Output {
    BufferTp buffer;
    AlphaTp  alpha;
  };

  const BufferTp &background,        &foreground;
  const AlphaTp  &background_alpha,  &foreground_alpha;

  std::uint64_t x_offset = 0;
  std::uint64_t y_offset = 0;

  Blending::Tp blending = Blending::Normal;

  Output Process() const {
    Output output;
    output.buffer = background;
    output.alpha  = background_alpha;

    for (auto bg_row = y_offset; 
              bg_row < y_offset + foreground.GetRCount();
              bg_row++) {
      for (auto bg_column = x_offset; 
                bg_column < x_offset + foreground.GetCCount();
                bg_column++) {

        auto fg_row    = bg_row    - y_offset;
        auto fg_column = bg_column - x_offset;

        output.buffer(bg_row, bg_column) = BlendAlpha(
          background(bg_row, bg_column),
          BlendElement(
            background(bg_row, bg_column),
            foreground(fg_row, fg_column),
            foreground_alpha(fg_row, fg_column),
            blending
          ),
          foreground_alpha(fg_row, fg_column)
        );
        output.alpha(bg_row, bg_column) = std::max(
          *background_alpha(bg_row, bg_column), 
          *foreground_alpha(fg_row, fg_column)
        );
      }
    }
    return output;
  }

private:

  ElementTp BlendAlpha(
    const ElementTp      &background,
    const ElementTp      &foreground,
    const AlphaElementTp &alpha
  ) const {
    ElementTp output;
    float     alphaf = static_cast<float>(*alpha) / Depth::Max<DepthV>;

    for (auto index = 0u;
              index < Color::ChannelCount<ColorV>;
              index++) {
      output[index] = 
        (background[index] * (1.0f - alphaf)) + 
        (foreground[index] * (       alphaf));
    }
    return output;
  }

  ElementTp BlendNormal(
    const ElementTp &background,
    const ElementTp &foreground
  ) const {
    return foreground;
  }

  ElementTp BlendDissolve(
    const ElementTp &background,
    const ElementTp &foreground,
    const AlphaElementTp &alpha
  ) const {
    static bool initialized = false;

    if (!initialized) {
      std::srand(std::time(0)); initialized = true;
    }
    auto random = static_cast<Depth::Underlying<DepthV>>(
      (static_cast<float>(std::rand()) / RAND_MAX) * Depth::Max<DepthV>
    );
    if (random < *alpha) {
      return foreground;
    }
    return background;
  }

  ElementTp BlendDarken(
    const ElementTp &background,
    const ElementTp &foreground
  ) const {
    ElementTp output;
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV>;
              index++) {
      output[index] = std::min(background[index], foreground[index]);
    }
    return output;
  }

  ElementTp BlendMultiply(
    const ElementTp &background,
    const ElementTp &foreground
  ) const {
    ElementTp output;
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV>;
              index++) {
      output[index] = background[index] * foreground[index] / 255;
    }
    return output;
  }

  ElementTp BlendColorBurn(
    const ElementTp &background,
    const ElementTp &foreground
  ) const {
    ElementTp output;
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV>;
              index++) {
      if (!foreground[index]) {
        output[index] = 0;
        continue;
      } 
      auto max  = Depth::Max<DepthV>;
      auto temp = static_cast<std::uint64_t>(max - background[index]) * max / foreground[index];
      output[index] = (temp > max) 
         ? 0 
         : max - temp;
    }
    return output;
  }

  ElementTp BlendLinearBurn(
    const ElementTp &background,
    const ElementTp &foreground
  ) const {
    ElementTp output;
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV>;
              index++) {
      output[index] = std::max(
        0l,
        static_cast<std::int64_t>(background[index]) + 
        static_cast<std::int64_t>(foreground[index]) - Depth::Max<DepthV>);
    }
    return output;
  }

  ElementTp BlendDarkerColor(
    const ElementTp &background,
    const ElementTp &foreground
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
    return CalculateLuminance(background) > CalculateLuminance(foreground)
      ? foreground 
      : background;
  }

  ElementTp BlendLighten(
    const ElementTp &background,
    const ElementTp &foreground
  ) const {
    ElementTp output;
    for (auto index = 0u;
              index < Color::ChannelCount<ColorV>;
              index++) {
      output[index] = std::max(background[index], foreground[index]);
    }
    return output;
  }

  ElementTp BlendElement(
    const ElementTp &background,
    const ElementTp &foreground,
    const AlphaElementTp &alpha,
    Blending::Tp blending
  ) const {
    ElementTp output;

    switch (blending) {
      case Blending::Normal      : return BlendNormal      (background, foreground);
      case Blending::Dissolve    : return BlendDissolve    (background, foreground, alpha);
      case Blending::Darken      : return BlendDarken      (background, foreground);
      case Blending::Multiply    : return BlendMultiply    (background, foreground);
      case Blending::ColorBurn   : return BlendColorBurn   (background, foreground);
      case Blending::LinearBurn  : return BlendLinearBurn  (background, foreground);
      case Blending::DarkerColor : return BlendDarkerColor (background, foreground);
      case Blending::Lighten     : return BlendLighten     (background, foreground);
      default: {
        throw std::runtime_error("blerr");
      }
    }
    return output;
  }

};
}; 
template <Depth::Tp DepthV, Color::Tp ColorV>
typename Processing::BlendClass<DepthV, ColorV>::Output
Blend(
  const Buffer<DepthV, ColorV>           &buffer_a,
  const Buffer<DepthV, Color::Grayscale> &alpha_a,
  const Buffer<DepthV, ColorV>           &buffer_b,
  const Buffer<DepthV, Color::Grayscale> &alpha_b,
  std::uint64_t x_offset,
  std::uint64_t y_offset,
  Blending::Tp  blending
) {
  return Processing::BlendClass<DepthV, ColorV>({
    buffer_a, 
    buffer_b,
    alpha_a, 
    alpha_b, 
    x_offset, 
    y_offset, 
    blending
  }).Process();
}

template <Depth::Tp DepthV, Color::Tp ColorV>
typename Processing::BlendClass<DepthV, ColorV>::Output
Blend(
  const Buffer<DepthV, ColorV>           &buffer_a,
  const Buffer<DepthV, Color::Grayscale> &alpha_a,
  const Buffer<DepthV, ColorV>           &buffer_b,
  const Buffer<DepthV, Color::Grayscale> &alpha_b,
  Blending::Tp  blending
) {
  return Processing::BlendClass<DepthV, ColorV>({
    buffer_a, 
    buffer_b,
    alpha_a, 
    alpha_b, 
    blending
  }).Process();
}

};
