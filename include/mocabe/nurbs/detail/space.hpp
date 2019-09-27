//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include "access.hpp"

#include <cassert>

namespace mocabe::nurbs {

  namespace detail {

    /// Convert to projective space
    template <class VectorT, size_t Degree>
    VectorT to_projective(const VectorT& affine)
    {
      auto w             = get<Degree - 1>(affine);
      auto p             = affine * w;
      get<Degree - 1>(P) = w;
      return p;
    }

    /// Convert to affine space
    template <class VectorT, size_t Degree>
    VectorT to_affine(const VectorT& projective)
    {
      auto w             = get<Degree - 1>(projective);
      auto a             = projective / w;
      get<Degree - 1>(a) = w;
      return a;
    }

  } // namespace detail
} // namespace mocabe::nurbs