/**
 * @file 
 * @brief Provides functions for encoding image buffers into various formats and saving to files.
 *
 * This header defines the encoding functionality for image buffers, including error handling
 * and support for different image formats. It includes functions to encode image data into
 * a byte sequence or directly to a file path with optional format specification.
 */
#pragma once

#include <image/core/buffer.h>
#include <image/io/export.h>
#include <image/io/error.h>
#include <image/io/format.h>
#include <file/type.h>
#include <filesystem>

namespace Image {
/**
 * @struct EncodeError
 * @brief Exception class for encoding-related errors.
 *
 * This class inherits from IoError and is used to handle errors that occur during
 * the image encoding process.
 */
struct EncodeError : IoError {
  /**
   * @brief Constructs an EncodeError with a specific error message.
   * @param message A string view containing the error message.
   */
  explicit EncodeError(
    std::string_view message

  ) : IoError(Message("EncodeError", message)) {}
};
/**
 * @brief Encodes an image buffer into a byte sequence with the specified format.
 * @param buffer The image buffer to encode.
 * @param format The target image format (e.g., PNG, JPEG).
 * @return A File::ByteSequence containing the encoded image data.
 * @throws EncodeError if encoding fails.
 */
IMAGE_IO_EXPORT;
File::ByteSequence Encode(
  Buffer buffer, Format format
);
/**
 * @brief Encodes an image buffer and saves it to a file with the specified format.
 * @param buffer The image buffer to encode.
 * @param path The filesystem path where the encoded image will be saved.
 * @param format The target image format (e.g., PNG, JPEG).
 * @throws EncodeError if encoding or file writing fails.
 */
IMAGE_IO_EXPORT;
void Encode(
  Buffer buffer, const std::filesystem::path &path, Format format
);
/**
 * @brief Encodes an image buffer and saves it to a file, inferring format from the file extension.
 * @param buffer The image buffer to encode.
 * @param path The filesystem path where the encoded image will be saved.
 * @throws EncodeError if encoding or file writing fails, or if the format cannot be inferred.
 */
IMAGE_IO_EXPORT;
void Encode(
  Buffer buffer, const std::filesystem::path &path
);
};
