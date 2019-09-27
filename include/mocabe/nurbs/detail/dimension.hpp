//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include "access.hpp"

#include <type_traits>

namespace mocabe::nurbs {

  namespace detail {

    template <class VectorT, size_t Index, class = void>
    struct dimension_impl
    {
      static constexpr size_t value = Index;
    };

    template <class VectorT, size_t Index>
    struct dimension_impl<
      VectorT,
      Index,
      std::void_t<decltype(
        vector_access<VectorT, Index>::get(std::declval<const VectorT&>()))>>
    {
      static constexpr size_t value = dimension_impl<VectorT, Index + 1>::value;
    };

    template <class VectorT>
    struct dimension
    {
      static constexpr size_t value =
        dimension_impl<std::decay_t<VectorT>, 0>::value;
    };
  } // namespace detail

  /// Get dimension of vector type
  template <class VectorT>
  static constexpr size_t dimension_v = detail::dimension<VectorT>::value;

} // namespace mocabe::nurbs