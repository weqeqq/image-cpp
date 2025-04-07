
#pragma once

#include <stdexcept>

namespace Image {
class Error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};
}

