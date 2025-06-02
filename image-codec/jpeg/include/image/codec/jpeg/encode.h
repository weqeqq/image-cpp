/**
 * @file encode.h
 * @brief JPEG encoding functionality for image data.
 *
 * This header provides functionality for encoding image data into JPEG format.
 * It includes an error handling structure specific to encoding operations and
 * two overloads of the Encode function to support different encoding parameters.
 */
#pragma once

#include <file/type.h>
#include <image/codec/jpeg/export.h>
#include <image/codec/jpeg/error.h>
#include <image/codec/jpeg/color.h>
#include <image/codec/jpeg/sampling.h>

namespace Image::JpegCodec {
/**
 * @struct EncodeError
 * @brief Exception class for JPEG encoding errors.
 *
 * This structure inherits from JpegCodecError to provide specific error handling
 * for JPEG encoding operations.
 */
struct EncodeError : JpegCodecError {
  /**
   * @brief Constructs an EncodeError with a specific message.
   * @param message A string view containing the error message.
   */
  explicit EncodeError(
    std::string_view message

  ) : JpegCodecError(Message("EncodeError", message)) {}
};
/**
 * @brief Encodes image data into JPEG format with specified parameters.
 *
 * @param data         Input image data as a byte span.
 * @param row_count    Number of rows in the image.
 * @param column_count Number of columns in the image.
 * @param color        Color format of the input image.
 * @param sampling     Chroma subsampling mode for the JPEG encoding.
 * @param qual         Quality factor for JPEG compression (1-100).
 *
 * @return A ByteSequence containing the encoded JPEG data.
 * @throws EncodeError if encoding fails.
 */
IMAGE_CODEC_JPEG_EXPORT;
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count, 
  std::int32_t   column_count, 
  Color          color,
  Sampling       sampling,
  int            qual 
);
/**
 * @brief Encodes image data into JPEG format with default parameters.
 *
 * @param data Input   image data as a byte span.
 * @param row_count    Number of rows in the image.
 * @param column_count Number of columns in the image.
 *
 * @return A ByteSequence containing the encoded JPEG data.
 * @throws EncodeError if encoding fails.
 *
 * @note Uses default color, sampling, and quality settings.
 */
IMAGE_CODEC_JPEG_EXPORT;
File::ByteSequence Encode(
  File::ByteSpan data,
  std::int32_t   row_count,
  std::int32_t   column_count
);
};
