#pragma once

#include <type_traits>

namespace nurbs{
template <class T>
struct knot_traits{
  using knot_type = T;
  static_assert(std::is_floating_point<T>::value,"knot_type should be floating point");
};
}