/**
 * @file 
 * @brief WebP image codec for decoding WebP images into byte sequences.
 *
 * This header defines the WebP codec functionality for decoding WebP image data,
 * including error handling and color space specifications.
 */
#pragma once 

#include <file/type.h>
#include <image/codec/webp/error.h>
#include <image/codec/webp/color.h>
#include <image/codec/webp/export.h>

namespace Image::WebpCodec {
/**
 * @brief Exception class for WebP decoding errors.
 *
 * Inherits from WebpCodecError to provide specific error handling for decoding operations.
 */
struct DecodeError : WebpCodecError {
  /**
   * @brief Constructs a DecodeError with a specified message.
   * @param message A string view containing the error message.
   */
  explicit DecodeError(
    std::string_view message

  ) : WebpCodecError(Message("DecodeError", message)) {}
};
/**
 * @brief Decodes WebP image data into a byte sequence with specified color format.
 *
 * @param data         Input WebP image data as a byte span.
 * @param row_count    Output parameter for the number of rows in the decoded image.
 * @param column_count Output parameter for the number of columns in the decoded image.
 * @param color        The color format to use for decoding.
 *
 * @return File::ByteSequence The decoded image data as a byte sequence.
 * @throws DecodeError If decoding fails.
 */
IMAGE_CODEC_WEBP_EXPORT;
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count,
  Color          color
);
/**
 * @brief Decodes WebP image data into a byte sequence with default color format.
 *
 * @param data         Input WebP image data as a byte span.
 * @param row_count    Output parameter for the number of rows in the decoded image.
 * @param column_count Output parameter for the number of columns in the decoded image.
 *
 * @return File::ByteSequence The decoded image data as a byte sequence.
 * @throws DecodeError If decoding fails.
 */
IMAGE_CODEC_WEBP_EXPORT;
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
);
};
