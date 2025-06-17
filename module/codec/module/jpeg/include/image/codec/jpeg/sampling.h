/**
 * @file JpegCodec.hpp
 * @brief Defines the Sampling enumeration for JPEG codec configuration.
 *
 * This header file contains the Sampling enumeration used to specify
 * chroma subsampling schemes for JPEG image compression within the
 * Image::JpegCodec namespace.
 */
#pragma once

namespace Image::JpegCodec {
/**
 * @enum Sampling
 * @brief Specifies chroma subsampling schemes for JPEG compression.
 *
 * This enumeration defines various chroma subsampling formats used in
 * JPEG image compression. Each value represents a specific subsampling
 * ratio for the chrominance components relative to the luminance component.
 */
enum class Sampling {
  S444,  /**< 4:4:4 sampling (no chroma subsampling, full resolution).                */
  S422,  /**< 4:2:2 sampling (horizontal subsampling by a factor of 2).               */
  S420,  /**< 4:2:0 sampling (horizontal and vertical subsampling by a factor of 2).  */
  SGray, /**< Grayscale (no chroma components, luminance only).                       */
  S440,  /**< 4:4:0 sampling (vertical subsampling by a factor of 2).                 */
  S411,  /**< 4:1:1 sampling (horizontal subsampling by a factor of 4).               */
  S441   /**< 4:4:1 sampling (horizontal subsampling by a factor of 4, less common).  */
};
};
