/**
 * @file decode.h
 * @brief JPEG codec functionality for decoding JPEG images.
 *
 * This header provides the interface for decoding JPEG images, including error handling
 * and data structures for managing decoded image data.
 */
#pragma once

#include <file/type.h>
#include <image/codec/jpeg/export.h>
#include <image/codec/jpeg/error.h>
#include <image/codec/jpeg/color.h>
#include <image/codec/jpeg/sampling.h>

namespace Image::JpegCodec {
/**
 * @class DecodeError
 * @brief Exception class for JPEG decoding errors.
 *
 * Inherits from JpegCodecError to provide specific error handling for JPEG decoding operations.
 */
struct DecodeError : JpegCodecError {
  /**
   * @brief Constructs a DecodeError with a specific message.
   * @param message A string view containing the error message.
   */
  explicit DecodeError(
    std::string_view message

  ) : JpegCodecError(Message("DecodeError", message)) {}
};
/**
 * @brief Decodes a JPEG image from a byte sequence.
 *
 * This function decodes a JPEG image provided as a byte span and outputs the decoded image
 * dimensions and properties.
 *
 * @param data         Input byte span containing the JPEG image data.
 * @param row_count    Reference to store the number of rows (height) of the decoded image.
 * @param column_count Reference to store the number of columns (width) of the decoded image.
 * @param color        Specifies the color format of the decoded image.
 * @param sampling     Reference to store the sampling factor used in the JPEG compression.
 *
 * @return File::ByteSequence The decoded image data as a byte sequence.
 * @throws DecodeError If an error occurs during decoding.
 */
IMAGE_CODEC_JPEG_EXPORT;
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count,
  Color          color,
  Sampling      &sampling
);
IMAGE_CODEC_JPEG_EXPORT;
File::ByteSequence Decode(
  File::ByteSpan data,
  std::int32_t  &row_count,
  std::int32_t  &column_count
);
};
