#pragma once

#include <cstdint>
#include <array>

#include <image/core/color.h>
#include <image/core/depth.h>

namespace Image {

template <Depth::Tp depth, Color::Tp color>
class Element {
public:

  template <typename>
  class Iterator;

  static constexpr Depth::Tp depth_value = depth;
  static constexpr Color::Tp color_value = color;

  static constexpr std::uint64_t channel_count = Color::ChannelCount<color>;
  static constexpr std::uint64_t bcount        = sizeof(Depth::Underlying<depth>) * channel_count;
  static constexpr std::uint64_t BCount        = sizeof(Depth::Underlying<depth>) * channel_count;

  Element() = default;
  Element(Depth::Underlying<depth> value) : data_(CreateData(value)) {}

  Element<depth, color> &operator=(Depth::Underlying<depth> value) {
    data_ = CreateData(value);
    return *this;
  }

  Depth::Underlying<depth> &operator*() {
    return data_[0];
  }
  const Depth::Underlying<depth> &operator*() const {
    return data_[0];
  }

  operator Depth::Underlying<depth>() {
    return data_[0];
  }

  template <typename... Args>
  Depth::Underlying<depth> &operator[](std::uint64_t index) {
    return data_[index];
  }
  const Depth::Underlying<depth> &operator[](std::uint64_t index) const {
    return data_[index];
  }

private:
  using ContainerTp = std::array<Depth::Underlying<depth>, Color::ChannelCount<color>>; 

  ContainerTp data_;

  ContainerTp CreateData(Depth::Underlying<depth> value) {
    ContainerTp data;
    for (auto &channel : data) {
      channel = value;
    }
    return data;
  }
};
template <Depth::Tp depth, Color::Tp color>
template <typename ElementTy>
class Element<depth, color>::Iterator {
public:
  explicit Iterator(ElementTy &container, std::uint64_t index) 
    : container_ (container)
    , index_     (index) {}

  std::conditional_t<!std::is_const_v<ElementTy>,
          Depth::Underlying<depth> &,
    const Depth::Underlying<depth> &>
  operator*() const {
    return container_[index_];
  }

  Iterator<ElementTy> &operator++() {
    index_++;
    return *this;
  } 
  Iterator<ElementTy> operator++(int) {
    Iterator<ElementTy> temp(container_, index_);
    operator++();
    return temp;
  }

  bool operator==(const Iterator<ElementTy> &other) const {
    return index_ == other.index_;
  }
  bool operator!=(const Iterator<ElementTy> &other) const {
    return !operator==(other);
  }

private:
  ElementTy    &container_;
  std::uint64_t index_;
};

template <Depth::Tp depth, Color::Tp color> 
static typename Element<depth, color>::template Iterator<Element<depth, color>> begin(Element<depth, color> &element) {
  return typename Element<depth, color>::template Iterator<Element<depth, color>>(element, 0);
}
template <Depth::Tp depth, Color::Tp color> 
static typename Element<depth, color>::template Iterator<const Element<depth, color>> begin(const Element<depth, color> &element) {
  return typename Element<depth, color>::template Iterator<const Element<depth, color>>(element, 0);
}

template <Depth::Tp depth, Color::Tp color> 
static typename Element<depth, color>::template Iterator<Element<depth, color>> end(Element<depth, color> &element) {
  return typename Element<depth, color>::template Iterator<Element<depth, color>>(element, element.channel_count);
}
template <Depth::Tp depth, Color::Tp color> 
static typename Element<depth, color>::template Iterator<const Element<depth, color>> end(const Element<depth, color> &element) {
  return typename Element<depth, color>::template Iterator<const Element<depth, color>>(element, element.channel_count);
}
};
