/**
 * @file 
 * @brief Provides functionality for encoding data into WebP format.
 *
 * This header defines the WebP codec namespace, including error handling and
 * encoding functions for converting raw image data into WebP format.
 */

#pragma once

#include <file/type.h>
#include <image/codec/webp/error.h>
#include <image/codec/webp/export.h>
#include <image/codec/webp/color.h>

namespace Image::WebpCodec {

/**
 * @class EncodeError
 * @brief Exception class for WebP encoding errors.
 *
 * This class inherits from WebpCodecError and is used to represent errors
 * that occur during the WebP encoding process.
 */
struct EncodeError : WebpCodecError {
  /**
   * @brief Constructs an EncodeError with a specified error message.
   * @param message A string view containing the error message.
   */
  explicit EncodeError(
    std::string_view message

  ) : WebpCodecError(Message("EncodeError", message)) {}
};

/**
 * @brief Encodes image data into WebP format with specified parameters.
 *
 * This function takes raw image data and encodes it into WebP format using
 * the provided dimensions, color format, and quality settings.
 *
 * @param data         The input image data as a byte span.
 * @param row_count    The number of rows in the image.
 * @param column_count The number of columns in the image.
 * @param color        The color format of the input image data.
 * @param qual         The quality factor for WebP encoding (typically 0-100).
 *
 * @return A ByteSequence containing the encoded WebP data.
 * @throws EncodeError If an error occurs during encoding.
 */
IMAGE_CODEC_WEBP_EXPORT;
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  Color          color,
  std::int32_t   qual
);

/**
 * @brief Encodes image data into WebP format with default parameters.
 *
 * This function takes raw image data and encodes it into WebP format using
 * the provided dimensions. Default color format and quality settings are used.
 *
 * @param data         The input image data as a byte span.
 * @param row_count    The number of rows in the image.
 * @param column_count The number of columns in the image.
 *
 * @return A ByteSequence containing the encoded WebP data.
 * @throws EncodeError If an error occurs during encoding.
 */
IMAGE_CODEC_WEBP_EXPORT;
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count
);
};
