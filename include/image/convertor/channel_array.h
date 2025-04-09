
#pragma once

#include <image/buffer.h>
#include <image/core/error.h>
#include <image/channel_array.h>

namespace Image {

template <typename InputT>
class ChannelArrayConvertor {
public:
  static_assert(false);
  ChannelArrayConvertor(InputT) {}
};
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
class ChannelArrayConvertor<Buffer<DepthV, ColorV, AlphaSetting>> {
public:

  using UsedBuffer       = Buffer       <DepthV, ColorV, AlphaSetting>;
  using UsedChannelArray = ChannelArray <DepthV, ColorV, AlphaSetting>; 

  explicit ChannelArrayConvertor(const UsedBuffer &input) : input_(input) {}

  UsedChannelArray Convert() const {
    UsedChannelArray output(
      input_.GetRowCount    (),
      input_.GetColumnCount ()
    );
    for (auto index = 0u;
              index < input_.GetLength();
              index++) {
      for (auto channel_index = 0u;
                channel_index < input_.ChannelCount;
                channel_index++) {
        output[channel_index][index] = input_[index][channel_index];
      }
    }
    return output;
  }

private:
  const UsedBuffer &input_;
};
template <Depth::Tp DepthV,
          Color::Tp ColorV,
          bool AlphaSetting>
class ChannelArrayConvertor<ChannelArray<DepthV, ColorV, AlphaSetting>> {
public:

  using UsedBuffer       = Buffer       <DepthV, ColorV, AlphaSetting>;
  using UsedChannelArray = ChannelArray <DepthV, ColorV, AlphaSetting>;

  explicit ChannelArrayConvertor(const UsedChannelArray &input) : input_(input) {}

  auto Convert() const {
    UsedBuffer output(
      input_.GetRowCount    (),
      input_.GetColumnCount ()
    );
    for (auto index = 0u;
              index < input_.GetLength();
              index++) {
      for (auto channel_index = 0u;
                channel_index < input_.ChannelCount;
                channel_index++) {
        output[index][channel_index] = input_[channel_index][index];
      }
    }
    return output;
  }  
private:
  const UsedChannelArray &input_;
};
};
