#pragma once
#include <type_traits>

namespace nurbs {

template <class T>
struct dimension {};

template <class T>
static constexpr int dimension_v = dimension<T>::value;

template <class T, class = void>
struct has_dimension : std::false_type {};

template <class T>
struct has_dimension<T, std::void_t<decltype(dimension<T>::value)>>
  : std::true_type {};

template <class T>
static constexpr bool has_dimension_v = has_dimension<T>::value;
} // namespace nurbs