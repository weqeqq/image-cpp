/**
 * @file color.h
 * @brief Defines the Color enumeration for JPEG codec image formats.
 *
 * This header file contains the Color enumeration used within the Image::JpegCodec
 * namespace to specify various color formats supported by the JPEG codec.
 */
#pragma once

namespace Image::JpegCodec {
/**
 * @enum Color
 * @brief Specifies the color format of an image for JPEG encoding and decoding.
 *
 * This enumeration defines the supported color formats for image data processed
 * by the JPEG codec. Each value represents a specific pixel format, including
 * various RGB, grayscale, and CMYK arrangements.
 */
enum class Color {
  Rgb,   /**< Standard RGB format (Red, Green, Blue).       */
  Bgr,   /**< BGR format (Blue, Green, Red).                */
  Rgbx,  /**< RGBX format (Red, Green, Blue, padding byte). */
  Bgrx,  /**< BGRX format (Blue, Green, Red, padding byte). */
  Xbgr,  /**< XBGR format (padding byte, Blue, Green, Red). */
  Xrgb,  /**< XRGB format (padding byte, Red, Green, Blue). */
  Gray,  /**< Grayscale format (single-channel intensity).  */
  Rgba,  /**< RGBA format (Red, Green, Blue, Alpha).        */
  Bgra,  /**< BGRA format (Blue, Green, Red, Alpha).        */
  Abgr,  /**< ABGR format (Alpha, Blue, Green, Red).        */
  Argb,  /**< ARGB format (Alpha, Red, Green, Blue).        */
  Cmyk,  /**< CMYK format (Cyan, Magenta, Yellow, Black).   */
};
};
