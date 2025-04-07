
#pragma once

#include <image/convertor/depth.h>
#include <image/convertor/color.h>

namespace Image {

template <typename InputT>
class DataConvertor {
public:

  static_assert(false);
  explicit DataConvertor(InputT) {} 
};

template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
class DataConvertor<Element<DepthV, ColorV, AlphaSetting>> {
public:
  using UsedElement = Element<DepthV, ColorV, AlphaSetting>;

  explicit DataConvertor(const UsedElement &input) : input_(input) {}

  template <Depth::Tp OutputDepthV,
            Color::Tp OutputColorV>
  decltype(auto) Convert() const {
    return DepthConvertor(ColorConvertor(input_)
      .template Convert<OutputColorV>())
      .template Convert<OutputDepthV>();
  }
private:
  const UsedElement &input_;
};

template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
class DataConvertor<Buffer<DepthV, ColorV, AlphaSetting>> {
public:

  template <Depth::Tp CustomDepthV, 
            Color::Tp CustomColorV>
  using CustomBuffer = Buffer<CustomDepthV, CustomColorV, AlphaSetting>;

  using UsedBuffer = Buffer<DepthV, ColorV, AlphaSetting>;

  explicit DataConvertor(const UsedBuffer &input) : input_(input) {}

  template <Depth::Tp OutputDepthV, 
            Color::Tp OutputColorV>
  decltype(auto) Convert() const {
    CustomBuffer<OutputDepthV, OutputColorV> output(
      input_.GetRowCount    (),
      input_.GetColumnCount ()
    );
    for (auto index = 0u;
              index < output.GetLength();
              index++) {
      output[index] = Image::DataConvertor(input_[index]).template Convert<OutputDepthV, OutputColorV>();
    }
    return output;
  }
private:
  const UsedBuffer &input_;
};
};
