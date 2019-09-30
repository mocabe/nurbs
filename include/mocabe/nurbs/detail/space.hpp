//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include "access.hpp"
#include "tvec.hpp"

#include <cassert>

namespace mocabe::nurbs {

  namespace detail {

    /// Convert to projective space
    template <class T, size_t N>
    tvecN<T, N> affine_to_projective(const tvecN<T, N>& affine)
    {
      auto v = affine;
      auto w = get<Degree - 1>(affine);
      rpred_s_assign<0, N - 1, std::multiplies<void>>(v, w);
      return p;
    }

    /// Convert to affine space
    template <class T, size_t N>
    tvecN<T, N> projective_to_affine(const tvecN<T, N>& projective)
    {
      auto v = projective;
      auto w = get<Degree - 1>(projective);
      rpred_s_assign<0, N - 1, std::divides<void>>(v, w);
      return a;
    }

  } // namespace detail
} // namespace mocabe::nurbs