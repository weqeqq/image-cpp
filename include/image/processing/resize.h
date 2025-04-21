
#pragma once

#include <image/buffer.h>

namespace Image {

class Interpolation {
public:
  enum Tp {
    NearestNeighbor,
    Bilinear,
  };
};

namespace Processing {

template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
class ResizeClass {
public:
  using UsedBuffer  = Buffer<DepthV, ColorV, AlphaSetting>;
  using UsedElement = Element<DepthV, ColorV, AlphaSetting>;

  using Value = Depth::Underlying<DepthV>;

  UsedBuffer        input_;
  Interpolation::Tp interpolation_;

  std::uint64_t row_count_;
  std::uint64_t column_count_;

  auto Process() const {
    switch (interpolation_) {
      case Interpolation::NearestNeighbor          : return NearestNeighbor();
      case Interpolation::Bilinear                 : return Bilinear();
      default: throw std::runtime_error("undefined interpolation");
    }
  }
private:
  inline auto ComputeRatio(std::uint64_t a, std::uint64_t b) const {
    return static_cast<float>(a - 1) / (b > 1 ? b - 1 : 1);
  }
  inline auto ComputeXRatio() const {
    return ComputeRatio(input_.GetColumnCount(), column_count_);
  }
  inline auto ComputeYRatio() const {
    return ComputeRatio(input_.GetRowCount(), row_count_);
  }
  UsedBuffer NearestNeighbor() const {
    UsedBuffer output(row_count_, column_count_); 

    if (output.IsEmpty()) {
      return output;
    }
    auto x_ratio = ComputeXRatio();
    auto y_ratio = ComputeYRatio();

    for (auto output_y = 0u; 
              output_y < row_count_; 
              output_y++) {

      for (auto output_x = 0u; 
                output_x < column_count_; 
                output_x++) {

        auto input_x = output_x * x_ratio;
        auto input_y = output_y * y_ratio;

        for (auto index = 0u;
                  index < UsedElement::ChannelCount;
                  index++) {
          output(output_y, output_x)[index] = input_(input_y, input_x)[index];
        }
      }
    }
    return output;
  }
  inline auto CheckYBounds(std::uint64_t value) const {
    return std::min(
      value,
      input_.GetRowCount() - 1
    );
  }
  std::tuple<
    std::uint64_t,
    std::uint64_t,
    float> ComputeYCoordinates(std::uint64_t output_y, float y_ratio) const {
    auto y_float = output_y * y_ratio;
    auto y_int   = CheckYBounds(y_float);
    auto y_frac  = y_float - y_int;
    auto y_next  = CheckYBounds(y_int + 1);

    return { y_int, y_next, y_frac };
  }
  inline auto CheckXBounds(std::uint64_t value) const {
    return std::min(
      value, 
      input_.GetColumnCount() - 1
    );
  }
  std::tuple<
    std::uint64_t,
    std::uint64_t,
    float> ComputeXCoordinates(std::uint64_t output_x, float x_ratio) const {
    auto x_float = output_x * x_ratio;
    auto x_int   = CheckXBounds(x_float);
    auto x_frac  = x_float - x_int;
    auto x_next  = CheckXBounds(x_int + 1);

    return { x_int, x_next, x_frac };
  }
  auto InterpolateChannel(
    std::uint64_t y_int  , std::uint64_t x_int,
    std::uint64_t y_next , std::uint64_t x_next,
    float         y_frac , float         x_frac,
    std::uint64_t channel
  ) const {
    const float v00 = input_(y_int, x_int)[channel];
    const float v01 = input_(y_int, x_next)[channel];
    const float v10 = input_(y_next, x_int)[channel];
    const float v11 = input_(y_next, x_next)[channel];

    const float top    = v00 * (1 - x_frac) + v01 * x_frac;
    const float bottom = v10 * (1 - x_frac) + v11 * x_frac;

    return top * (1 - y_frac) + bottom * y_frac;
  }
  UsedBuffer Bilinear() const {
    UsedBuffer output(row_count_, column_count_);

    if (output.IsEmpty()) {
      return output;
    }
    const float x_ratio = ComputeXRatio();
    const float y_ratio = ComputeYRatio();

    for (auto output_y = 0u; 
              output_y < row_count_;
              output_y++) {
      const auto [y_int, y_next, y_frac] = ComputeYCoordinates(output_y, y_ratio);

      for (auto output_x = 0u; 
                output_x < column_count_; 
                output_x++) {
        const auto [x_int, x_next, x_frac] = ComputeXCoordinates(output_x, x_ratio);

        for (auto channel = 0u; 
                  channel < UsedElement::ChannelCount;
                  channel++) {
          output(output_y, output_x)[channel] = InterpolateChannel(
            y_int  , x_int, 
            y_next , x_next,
            y_frac , x_frac,
            channel
          );
        }
      }
    }
    return output;
  }
};
};
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
static auto Resize(
  const Buffer<DepthV, ColorV, AlphaSetting> &buffer,
  std::uint64_t     row_count, 
  std::uint64_t     column_count, 
  Interpolation::Tp interpolation) {
  return Processing::ResizeClass<DepthV, ColorV, AlphaSetting>({
    buffer,
    interpolation,
    row_count,
    column_count
  }).Process();
}
}
