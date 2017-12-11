#pragma once
#include "access.hpp"
#include "dimension.hpp"

namespace nurbs {
// element_type_t
template <class V, size_t N>
using element_type_t =
  std::decay_t<decltype(nurbs::get<N>(std::declval<V>()))>;
}

namespace nurbs::detail {
template <class V, size_t N>
struct has_single_type {
  using type = decltype(get<N - 1>(std::declval<V>()));
  static constexpr bool value =
    has_single_type<V, N - 1>::value &&
    std::is_same_v<type, typename has_single_type<V, N - 1>::type>;
};

template <class V>
struct has_single_type<V, 1> : std::true_type {
  using type = decltype(get<0>(std::declval<V>()));
};
} // namespace nurbs::detail

namespace nurbs {
// has_single_type_v
template <class V>
constexpr bool has_single_type_v =
  detail::has_single_type<V, dimension_v<V>>::value;
}