
#pragma once

#include <stdexcept>

namespace Image {
class Error : public std::runtime_error {
public:
  Error(const std::string &message) : std::runtime_error(message) {}
protected:
  static std::string Concat(
    const std::string &first,
    const std::string &second
  ) {
    return first + " " + second;
  }
}; // class Error
}; // namespace Image
