/**
 * @file
 * @brief Defines the Color enumeration for the WebpCodec namespace.
 *
 * This header file contains the Color enumeration used within the WebpCodec namespace
 * to specify different color formats for image processing.
 */
#pragma once

namespace Image::WebpCodec {
/**
 * @enum Color
 * @brief Specifies the color format for WebP image encoding and decoding.
 *
 * This enumeration defines the supported color formats for WebP images, including
 * various RGB and RGBA formats with different byte orders.
 */
enum class Color {
  Bgr,  /**< Blue-Green-Red color format (24-bit).        */
  Bgra, /**< Blue-Green-Blue-Alpha color format (32-bit). */
  Rgb,  /**< Red-Green-Blue color format (24-bit).        */
  Rgba, /**< Red-Green-Blue-Alpha color format (32-bit).  */
  Argb  /**< Alpha-Red-Green-Blue color format (32-bit).  */
};
};
