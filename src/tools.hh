#pragma once

#include <units.h>
using namespace units;

namespace tools
{
  template <class T>
  T round_down(const T& a, const T& b) {
    return int(a / b) * b;
  }
  std::string pretty_print(time::second_t t);
}
