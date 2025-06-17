/**
 * @file
 * @brief Provides functions for decoding image data from various sources.
 *
 * This header defines the interface for decoding image data into a Buffer object.
 * It supports decoding from raw byte spans and file paths, with optional format
 * specification. Errors during decoding are reported via the DecodeError class.
 */
#pragma once 

#include <image/core/buffer.h>
#include <file/read.h>
#include <filesystem>
#include <image/io/export.h>
#include <image/io/format.h>
#include <image/io/error.h>

namespace Image {
/**
 * @class DecodeError
 * @brief Exception class for errors occurring during image decoding.
 *
 * Inherits from IoError to provide specific error handling for decoding operations.
 */
struct DecodeError : IoError {
  /**
   * @brief Constructs a DecodeError with a specific message.
   * @param message A string view containing the error message.
   */
  explicit DecodeError(
    std::string_view message
  ) : IoError(Message("DecodeError", message)) {}
};
/**
 * @brief Decodes image data from a byte span with a specified format.
 * @param data The byte span containing the image data.
 * @param format The format of the image data.
 * @return Buffer containing the decoded image data.
 * @throws DecodeError if decoding fails.
 */
IMAGE_IO_EXPORT;
Buffer Decode(File::ByteSpan data, Format format);
/**
 * @brief Decodes image data from a byte span with automatic format detection.
 * @param data The byte span containing the image data.
 * @return Buffer containing the decoded image data.
 * @throws DecodeError if decoding fails or format cannot be detected.
 */
IMAGE_IO_EXPORT;
Buffer Decode(File::ByteSpan data);
/**
 * @brief Decodes image data from a file path with a specified format.
 * @param path The filesystem path to the image file.
 * @param format The format of the image data.
 * @return Buffer containing the decoded image data.
 * @throws DecodeError if decoding fails or the file cannot be read.
 */
IMAGE_IO_EXPORT;
Buffer Decode(const std::filesystem::path &path, Format format);
/**
 * @brief Decodes image data from a file path with automatic format detection.
 * @param path The filesystem path to the image file.
 * @return Buffer containing the decoded image data.
 * @throws DecodeError if decoding fails or the file cannot be read.
 */
IMAGE_IO_EXPORT;
Buffer Decode(const std::filesystem::path &path);

}
