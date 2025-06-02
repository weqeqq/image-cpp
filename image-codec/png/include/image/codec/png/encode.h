/**
 * @file
 * @brief Provides functionality for encoding PNG images.
 *
 * This header defines the EncodeError structure and functions for encoding image data
 * into PNG format within the Image::PngCodec namespace.
 */
#pragma once

#include <file/type.h>
#include <image/codec/png/export.h>
#include <image/codec/png/error.h>
#include <image/codec/png/color.h>

namespace Image::PngCodec {
/**
 * @struct EncodeError
 * @brief Exception class for PNG encoding errors.
 * @details Inherits from PngCodecError to provide specific error handling for PNG encoding operations.
 */
struct EncodeError : PngCodecError {
  /**
   * @brief Constructs an EncodeError with a specific error message.
   * @param message A string view containing the error message.
   */
  explicit EncodeError(
    std::string_view message

  ) : PngCodecError(Message("EncodeError", message)) {}
};

/**
 * @brief Encodes image data into PNG format with specified color type.
 *
 * @param data         The input image data as a byte span.
 * @param row_count    The number of rows in the image.
 * @param column_count The number of columns in the image.
 * @param color        The color type of the image.
 *
 * @return A ByteSequence containing the encoded PNG data.
 * @throws EncodeError if the encoding process fails.
 */
IMAGE_CODEC_PNG_EXPORT;
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count,
  Color          color
);
/**
 * @brief Encodes image data into PNG format with default color type.
 *
 * @param data         The input image data as a byte span.
 * @param row_count    The number of rows in the image.
 * @param column_count The number of columns in the image.
 *
 * @return A ByteSequence containing the encoded PNG data.
 * @throws EncodeError if the encoding process fails.
 */
IMAGE_CODEC_PNG_EXPORT;
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count
);
};
