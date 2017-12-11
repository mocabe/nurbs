#pragma once
#include <cmath>
#include "access.hpp"
#include "dimension.hpp"

// detect macro
namespace nurbs::detail {
// FP_FAST_FMA
#if defined(FP_FAST_FMA)
constexpr bool fp_fast_fma = true;
#else
constexpr bool fp_fast_fma = false;
#endif
// FP_FAST_FMAF
#if defined(FP_FAST_FMAF)
constexpr bool fp_fast_fmaf = true;
#else
constexpr bool fp_fast_fmaf = false;
#endif
// FP_FAST_FMAL
#if defined(FP_FAST_FMAL)
constexpr bool fp_fast_fmal = true;
#else
constexpr bool fp_fast_fmal = false;
#endif

template <class FP>
constexpr bool fast_fma_enabled = (std::is_same_v<double, FP> && fp_fast_fma) ||
                                  (std::is_same_v<float, FP> && fp_fast_fmaf) ||
                                  (std::is_same_v<long double, FP> &&
                                   fp_fast_fmal);
}

// detail
namespace nurbs::detail {
template <size_t N, class FP, class V>
struct fma_division_impl {
  static void fma_division(FP a, const V& x, const V& y, V& result) {
    auto yval = get<N - 1>(y);
    auto xval = get<N - 1>(x);
    // Some compilers don't support FMA as library but still able to
    // auto-generate it from normal codes.
    if constexpr (has_single_type_v<V> && fast_fma_enabled<FP>)
      get<N - 1>(result) =
        std::fma(static_cast<element_type_t<V, 0>>(a), yval - xval, xval);
    else
      get<N - 1>(result) = a * (yval - xval) + xval;

    fma_division_impl<N - 1, FP, V>::fma_division(a, x, y, result);
  }
};

template <class FP, class V>
struct fma_division_impl<1, FP, V> {
  static void fma_division(FP a, const V& x, const V& y, V& result) {
    auto yval = get<0>(y);
    auto xval = get<0>(x);
    if constexpr (fast_fma_enabled<FP>)
      get<0>(result) = std::fma(a, yval - xval, xval);
    else
      get<0>(result) = a * (yval - xval) + xval;
  }
};
} // namespace nurbs::detail

// fma_division
namespace nurbs {
//! calculate (1-a) * x + a * y
template <
  class FP, class V,
  class = std::enable_if_t<std::is_floating_point_v<FP>>>
void fma_division(FP a, const V& x, const V& y, V& result) {
  detail::fma_division_impl<dimension_v<V>, std::decay_t<FP>, std::decay_t<V>>::
    fma_division(a, x, y, result);
};
} // namespace nurbs