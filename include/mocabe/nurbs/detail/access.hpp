//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace mocabe::nurbs {

  /// Default vector access trait
  template <class VectorT, size_t Degree>
  struct vector_access;

  /// Access to vector element
  template <size_t Degree, class VectorT>
  constexpr decltype(auto) get(VectorT&& t)
  {
    return vector_access<std::decay_t<VectorT>, Degree>::get(
      std::forward<VectorT>(t));
  }
} // namespace mocabe::nurbs