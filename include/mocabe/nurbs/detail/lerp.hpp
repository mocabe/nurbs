//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

namespace mocabe::nurbs {

  namespace detail {

    /// FMA lerp
    template <class KnotT>
    KnotT lerp(KnotT t, KnotT v0, KnotT v1)
    {
      return t * v1 - (t * v0 - v0);
    }

  } // namespace detail

} // namespace mocabe::nurbs