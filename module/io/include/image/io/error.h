/**
 * @file IoError.hpp
 * @brief Defines the IoError class for handling I/O-related errors.
 *
 * This file contains the IoError class, which inherits from the base Error class
 * and is used to represent errors specific to input/output operations within the
 * Image namespace.
 */
#pragma once

#include <image/core/error.h>

namespace Image {
/**
 * @class IoError
 * @brief A class representing I/O-specific errors.
 *
 * The IoError class extends the base Error class to provide functionality for
 * handling errors related to input/output operations. It constructs an error
 * with a specific namespace and message.
 */
class IoError : public Error {
protected:
  /**
   * @brief Constructs an IoError with a given message.
   * @param message A string view containing the error message.
   *
   * The constructor initializes the base Error class with a namespace-qualified
   * message, specifically for I/O-related errors.
   */
  explicit IoError(
    std::string_view message

  ) : Error(Namespace("IoError", message)) {}
};
};
