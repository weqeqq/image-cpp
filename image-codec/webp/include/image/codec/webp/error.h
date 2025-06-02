/**
 * @file
 * @brief Defines the WebpCodecError class for handling WebP codec-specific errors.
 *
 * This file contains the declaration of the WebpCodecError class, which extends
 * the base Error class to provide error handling specific to WebP codec operations.
 */
#pragma once

#include <image/core/error.h>
#include <string_view>

namespace Image {
/**
 * @class WebpCodecError
 * @brief A class representing errors specific to WebP codec operations.
 *
 * This class inherits from the base Error class and is used to encapsulate
 * error information related to WebP codec processing. It constructs an error
 * with a specific namespace and message for detailed error reporting.
 */
class WebpCodecError : public Error {
protected:
  /**
   * @brief Constructs a WebpCodecError with the specified error message.
   * @param message A string view containing the error message.
   */
  explicit WebpCodecError(
    std::string_view message

  ) : Error(Namespace("WebpCodecError", message)) {}
};
};
