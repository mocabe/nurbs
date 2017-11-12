#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <type_traits>

namespace nurbs {
  namespace detail{
  template <int Offset, class T, class S> struct Mult_impl {
    static void mult(T &t, const S &s) {
      get<Offset>(t) = get<Offset>(t) * s;
      Mult_impl<Offset - 1, T, S>::mult(t, s);
    }
  };

  template <class T, class S> struct Mult_impl<0, T, S> {
    static void mult(T &t, const S &s) { get<0>(t) = get<0>(t) * s; }
  };

  template <class T, class S> struct Mult {
    static void mult(T &t, const S &s) { Mult_impl<dimension_v<T> - 1, T, S>::mult(t, s); }
  };

  template <class T, class V> void mult(T &t, const V &v) { Mult<T, V>::mult(t, v); }

  } // namespace detail

  // ranged mult: mults v[0] to v[R] with s
  template <int R, class T, class S> void rmult(T &t, const S &s) {
    detail::Mult_impl<R, T, S>::mult(t, s);
  }

  template <class T, class S, class = std::enable_if_t<has_dimension_v<T>>>
  T operator*(const T &t, const S &v) {
    T r{t};
    detail::mult(r, v);
    return r;
  }

} // namespace nurbs