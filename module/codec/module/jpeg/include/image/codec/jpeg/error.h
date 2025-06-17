/**
 * @file
 * @brief Defines the JpegCodecError class for JPEG codec-related errors.
 */
#pragma once

#include <image/core/error.h>

namespace Image {
/**
 * @brief Exception class for JPEG codec-related errors.
 * 
 * This class represents errors specific to JPEG codec operations within the Image library.
 * It inherits from the base Error class and provides a specialized error namespace.
 * 
 * @note This class is protected and meant to be used as a base class for more specific JPEG codec error types.
 */
class JpegCodecError : public Error {
protected:
  /**
   * @brief Constructs a JpegCodecError with the given error message.
   * 
   * The constructor wraps the provided message in a "JpegCodecError" namespace
   * to clearly identify the error source.
   * 
   * @param message A string_view containing the error description.
   */
  explicit JpegCodecError(
    std::string_view message

  ) : Error(Namespace("JpegCodecError", message)) {}
};
};
