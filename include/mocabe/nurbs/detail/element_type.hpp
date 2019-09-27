//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include "dimension.hpp"

namespace mocabe::nurbs {

  namespace detail {
    template <class VectorT, size_t N>
    struct element_type_impl
    {
      using type = std::common_type_t<
        typename element_type_impl<VectorT, N - 1>::type,
        decltype(get<N>(std::declval<VectorT>()))>;
    };

    template <class VectorT>
    struct element_type_impl<VectorT, 0>
    {
      using type = decltype(get<0>(std::declval<VectorT>()));
    };

    template <class VectorT>
    struct element_type
    {
      using type =
        typename element_type_impl<VectorT, dimension_v<VectorT> - 1>::type;
    };
  } // namespace detail

  /// Common element type of vector type
  template <class VectorT>
  using element_type_t = typename detail::element_type<VectorT>::type;

  /// Common type of element type of vector type and knot type
  template <class VectorT, class KnotT>
  using compute_type_t = std::common_type_t<element_type_t<VectorT>, KnotT>;
}