/**
 * @file
 * @brief Defines the PngCodecError class for handling PNG codec-specific errors.
 *
 * This header file declares the PngCodecError class, which extends the base Error
 * class to provide error handling specific to the PNG codec within the Image
 * namespace.
 */
#pragma once

#include <image/core/error.h>

namespace Image::PngCodec {
/**
 * @class PngCodecError
 * @brief A class for representing errors specific to the PNG codec.
 *
 * The PngCodecError class inherits from the base Error class and is used to
 * encapsulate error information related to PNG codec operations. It provides a
 * constructor that takes an error message and associates it with the PngCodecError
 * namespace.
 */
class PngCodecError : public Error {
protected:
  /**
   * @brief Constructs a PngCodecError with a specified error message.
   * @param message A string view containing the error message.
   *
   * The constructor initializes the base Error class with a namespaced error
   * message, prefixing the provided message with "PngCodecError".
   */
  explicit PngCodecError(
    std::string_view message

  ) : Error(Namespace("PngCodecError", message)) {}
};
};
