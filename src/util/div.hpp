#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <type_traits>

namespace nurbs {
  namespace detail{
  template <int Offset, class T, class S> struct Div_impl {
    static constexpr void div(T &t, const S &s) {
      get<Offset>(t) = get<Offset>(t) / s;
      Div_impl<Offset - 1, T, S>::div(t, s);
    }
  };

  template <class T, class S> struct Div_impl<0, T, S> {
    static constexpr void div(T &t, const S &s) { get<0>(t) = get<0>(t) / s; }
  };

  template <class T, class S> constexpr void div(T &t, const S &s) {
    Div_impl<dimension_v<T> - 1, T, S>::div(t, s);
  }
  } //namespace nurbs::detail

  // ranged div: divides v[0] to v[R] with s
  template <int R, class T, class S> 
  constexpr void rdiv(T &t, const S &s) { detail::Div_impl<R, T, S>::div(t, s); }

  template <class T, class S, class = std::enable_if_t<has_dimension_v<T>>>
  constexpr T operator/(const T &t, const S &s) {
    T r{t};
    detail::div(r, s);
    return r;
  }

} //namespace nurbs