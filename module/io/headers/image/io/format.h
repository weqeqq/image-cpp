/**
 * @file image_format.hpp
 * @brief Defines the Format enum class for image file formats.
 *
 * This header file contains the declaration of the Format enum class,
 * which represents supported image file formats.
 */
#pragma once

namespace Image {
/**
 * @enum Format
 * @brief Represents supported image file formats.
 *
 * This enum class defines the possible image formats that can be used
 * within the Image namespace.
 */
enum class Format {
  Png,  /**< Portable Network Graphics format.               */
  Jpeg, /**< Joint Photographic Experts Group format.        */
  Webp  /**< WebP format, a modern image format for web use. */
};
};
