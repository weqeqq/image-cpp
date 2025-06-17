/**
 * @file 
 * @brief Defines the Color enumeration for PNG image color formats within the Image::PngCodec namespace.
 *
 * This header provides an enumeration of supported color formats for PNG image processing.
 */
#pragma once

#include <cstdint>

namespace Image::PngCodec {
/**
 * @enum Color
 * @brief Represents different color formats supported by the PNG codec.
 *
 * This enumeration defines various color formats used in PNG image processing,
 * such as BGR, BGRA, RGB, RGBA, and CMYK, each associated with a unique 64-bit
 * value for identification.
 */
enum class Color : std::uint32_t {
  Bgr  = 0x80000888, /**< Blue-Green-Red color format.                          */
  Bgra = 0x81008888, /**< Blue-Green-Red-Alpha color format with alpha channel. */
  Bgrx = 0x90008888, /**< Blue-Green-Red with padding byte.                     */
  Rgb  = 0xA0000888, /**< Red-Green-Blue color format.                          */
  Rgba = 0xA1008888, /**< Red-Green-Blue-Alpha color format with alpha channel. */
  Rgbx = 0xB0008888, /**< Red-Green-Blue with padding byte.                     */
  Cmyk = 0xD0038888, /**< Cyan-Magenta-Yellow-Black color format.               */
};
};
