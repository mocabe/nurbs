#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <cmath>
namespace nurbs::detail{
  template <size_t N, class FP, class V>
  struct fma_division_impl {
    static void fma_division(FP a, const V& x, const V& y, V& result) {
      get<N - 1>(result) =
          std::fma(a, get<N - 1>(y) - get<N - 1>(x), get<N - 1>(x));
      fma_division_impl<N - 1, FP, V>::fma_division(a, x, y, result);
    }
  };

  template <class FP, class V>
  struct fma_division_impl<1, FP, V> {
    static void fma_division(FP a, const V& x, const V& y, V& result) {
      get<0>(result) =
          std::fma(a, get<0>(y) - get<0>(x), get<0>(x));
    }
  };
}
namespace nurbs {
  //! calculate (1-a) * x + a * y
  template <class FP, class V, class = std::enable_if_t<std::is_floating_point_v<FP>>>
  void fma_division(FP a, const V& x, const V& y, V& result){
    detail::fma_division_impl<dimension_v<V>,FP,V>::fma_division(a, x, y, result);
  };
}