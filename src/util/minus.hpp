#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <type_traits>

namespace nurbs {
  namespace detail{
  template <int Offset, class V1, class V2> struct Minus_impl {
    static void minus(V1 &v1, const V2 &v2) {
      get<Offset>(v1) -= get<Offset>(v2);
      Minus_impl<Offset - 1, V1, V2>::minus(v1, v2);
    }
  };

  template <class V1, class V2> struct Minus_impl<0, V1, V2> {
    static void minus(V1 &v1, const V2 &v2) { get<0>(v1) -= get<0>(v2); }
  };

  template <class V1, class V2, class = std::enable_if_t<dimension_v<V1> == dimension_v<V2>>>
  struct Minus {
    static void minus(V1 &v1, const V2 &v2) {
      Minus_impl<dimension_v<V1> - 1, V1, V2>::minus(v1, v2);
    }
  };
  template <class V1, class V2> void minus(V1 &v1, const V2 &v2) { Minus<V1, V2>::minus(v1, v2); }
  }//namespace nurbs::detail

  template <class V1, class V2,
            class = std::enable_if_t<has_dimension_v<V1> && has_dimension_v<V2>>>
  V1 operator-(const V1 &v1, const V2 &v2) {
    V1 r{v1};
    detail::minus(r, v2);
    return r;
  }
} // namespace nurbs