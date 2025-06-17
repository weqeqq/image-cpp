/**
 * @file
 * @brief PNG codec for decoding PNG image data.
 *
 * This header defines the PNG decoding functionality, including error handling
 * and decoding operations for PNG images with specified or default color formats.
 */
#pragma once

#include <file/type.h>
#include <image/codec/png/color.h>
#include <image/codec/png/error.h>
#include <image/codec/png/export.h>

namespace Image::PngCodec {
/**
 * @struct DecodeError
 * @brief Error type for PNG decoding failures.
 *
 * Inherits from the base Error class to provide specific error handling
 * for PNG decoding operations.
 */
struct DecodeError : Error {
  /**
   * @brief Constructs a DecodeError with a specified message.
   * @param message A string view containing the error message.
   */
  explicit DecodeError(
    std::string_view message

  ) : Error(Message("DecodeError: ", message)) {}
};
/**
 * @brief Decodes PNG image data with specified color format.
 *
 * @param data          Input byte span containing PNG image data.
 * @param row_count     Reference to store the number of rows in the decoded image.
 * @param column_count  Reference to store the number of columns in the decoded image.
 * @param color         The color format to use for decoding.
 *
 * @return File::ByteSequence The decoded image data.
 * @throw DecodeError If decoding fails.
 */
IMAGE_CODEC_PNG_EXPORT;
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count,
  Color          color
);

/**
 * @brief Decodes PNG image data with default color format.
 *
 * @param data         Input byte span containing PNG image data.
 * @param row_count    Reference to store the number of rows in the decoded image.
 * @param column_count Reference to store the number of columns in the decoded image.
 *
 * @return File::ByteSequence The decoded image data.
 * @throw DecodeError If decoding fails.
 */
IMAGE_CODEC_PNG_EXPORT;
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
);
};
