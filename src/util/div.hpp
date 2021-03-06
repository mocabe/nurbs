#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <type_traits>

namespace nurbs::detail {
template <size_t N, class T, class S>
struct Div_impl {
  static constexpr void div(T &t, const S &s) {
    get<N - 1>(t) = get<N - 1>(t) / s;
    Div_impl<N - 1, T, S>::div(t, s);
  }
};

template <class T, class S>
struct Div_impl<1, T, S> {
  static constexpr void div(T &t, const S &s) { get<0>(t) = get<0>(t) / s; }
};

template <class T, class S>
struct Div_impl<0, T, S> {};

template <class T, class S>
constexpr void div(T &t, const S &s) {
  Div_impl<dimension_v<T>, T, S>::div(t, s);
}
} // namespace nurbs::detail

namespace nurbs {
//! ranged div: divides v[0] to v[N-1] with s
template <size_t N, class T, class S>
constexpr void rdiv(T &t, const S &s) {
  detail::Div_impl<N, T, S>::div(t, s);
}

template <class T, class S, class = std::enable_if_t<has_dimension_v<T>>>
constexpr T operator/(const T &t, const S &s) {
  T r{t};
  detail::div(r, s);
  return r;
}

} // namespace nurbs