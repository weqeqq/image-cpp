
#pragma once

#include <format>
#include <string_view>
#include <stdexcept>

namespace Image {
/**
 * @class Error
 * @brief Exception class for image processing errors, derived from std::runtime_error.
 *
 * This class provides a custom exception type for errors occurring within the Image namespace.
 * It formats error messages with a namespace prefix for better context.
 */
class Error : public std::runtime_error {
protected:
  /**
   * @brief Constructs an Error with a formatted message.
   * @param message The error message as a string view.
   *
   * The constructor prepends the "Image" namespace to the provided message
   * using the Namespace() function.
   */
  explicit Error(
    std::string_view message

  ) : std::runtime_error(Namespace("Image", message)) {}
  /**
   * @brief Formats a message with a namespace prefix.
   *
   * @param prefix The namespace prefix (e.g., "Image").
   * @param message The error message to format.
   *
   * @return A formatted string in the form "prefix::message".
   * This static utility function creates a namespaced error message.
   */
  static std::string Namespace(
    std::string_view prefix, 
    std::string_view message
  ) {
    return std::format("{}::{}", prefix, message);
  }
  /**
   * @brief Formats a message with a prefix and colon separator.
   *
   * @param prefix The prefix to prepend (e.g., "Image").
   * @param message The message to format.
   *
   * @return A formatted string in the form "prefix: message".
   * This static utility function creates a prefixed error message with a colon separator.
   */
  static std::string Message(
    std::string_view prefix, 
    std::string_view message
  ) {
    return std::format("{}: {}", prefix, message);
  }
};
};
