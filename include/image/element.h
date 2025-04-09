#pragma once

#include <cstdint>
#include <array>

#include <image/core/color.h>
#include <image/core/depth.h>

namespace Image {

static constexpr bool DisableAlpha = false;
static constexpr bool EnableAlpha  = true;

template <Depth::Tp DepthV  = DefDepth, 
          Color::Tp ColorV  = DefColor,
          bool AlphaSetting = DisableAlpha>
class Element {
public:

  static constexpr Depth::Tp DepthValue   = DepthV;
  static constexpr Color::Tp ColorValue   = ColorV;
  static constexpr bool      AlphaEnabled = AlphaSetting;

  static constexpr std::uint64_t ChannelCount = Color::ChannelCount<ColorV, AlphaSetting>;
  static constexpr std::uint64_t ByteCount    = Depth::Length<DepthV> * ChannelCount;

  using Value = Depth::Underlying<DepthV>;

  Element() = default;

  Element(Value value) {
    for (decltype(auto) channel : data_) { channel = value; }
  }

  bool operator==(const Element &other) const {
    return data_ == other.data_;
  }
  bool operator!=(const Element &other) const {
    return !operator==(other);
  }

  Element &operator=(Value value) {
    return *this = Element(value);
  }

  Value &operator*() {
    return data_[0];
  }
  Value operator*() const {
    return data_[0];
  }
  operator Value() const {
    return data_[0];
  }

  Value &operator[](std::uint64_t index) {
    return data_[index];
  }
  Value operator[](std::uint64_t index) const {
    return data_[index];
  }

private:
  using Container = std::array<
    Depth::Underlying   <DepthV>,
    Color::ChannelCount <ColorV, AlphaSetting>
  >;
public:
  using Iterator      = typename Container::iterator;
  using ConstIterator = typename Container::const_iterator;

  using RevIterator      = typename Container::reverse_iterator;
  using ConstRevIterator = typename Container::const_reverse_iterator;

  auto Begin() {
    return data_.begin();
  }
  auto Begin() const {
    return data_.begin();
  }
  auto ConstBegin() const {
    return data_.cbegin();
  }
  auto End() {
    return data_.end();
  }
  auto End() const {
    return data_.end();
  }
  auto ConstEnd() const {
    return data_.cend();
  }
  auto RevBegin() {
    return data_.rbegin();
  }
  auto RevBegin() const {
    return data_.rbegin();
  }
  auto ConstRevBegin() const {
    return data_.crbegin();
  }
  auto RevEnd() {
    return data_.rend();
  }
  auto RevEnd() const {
    return data_.rend();
  }
  auto ConstRevEnd() const {
    return data_.crend();
  }
private:
  Container data_;
};
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto begin(Element<DepthV, ColorV, AlphaSetting> &element) {
  return element.Begin();
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto begin(const Element<DepthV, ColorV, AlphaSetting> &element) {
  return element.Begin();
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto end(Element<DepthV, ColorV, AlphaSetting> &element) {
  return element.End();
}
template <Depth::Tp DepthV, 
          Color::Tp ColorV,
          bool AlphaSetting>
auto end(const Element<DepthV, ColorV, AlphaSetting> &element) {
  return element.End();
}

template <Depth::Tp DepthV = DefDepth,
          Color::Tp ColorV = DefColor>
class AlphaElement : public Element<DepthV, ColorV, EnableAlpha> {
public:
  using Element<DepthV, ColorV, EnableAlpha>::Element;
};

}
