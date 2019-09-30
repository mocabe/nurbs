//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

# pragma once

#include "access.hpp"
#include "tvec.hpp"

namespace mocabe::nurbs {

  template <class T, size_t N>
  struct tvecN;

  namespace detail {

    template <size_t Cur, size_t End, class Pred, class T, size_t N, class U>
    struct rpred_s_impl
    {
      static void apply(tvecN<T, N>& vec, const U& val)
      {
        if constexpr (Cur != End) {
          vec.values[Cur] = Pred()(vec.values[Cur], val);
          rpred_s_impl<Cur + 1, End, Pred, T, N, U>::apply(vec, val);
        }
      }
    };

    template <size_t Bgn, size_t End, class Pred, class T, size_t N, class U>
    void rpred_s_assign(tvecN<T, N>& vec, const U& val)
    {
      static_assert(0 <= Bgn);
      static_assert(Bgn <= End);
      static_assert(End <= N);
      rpred_s_impl<Bgn, End, Pred, T, N, U>::apply(vec, val);
    }

    template <size_t Cur, size_t End, class Pred, class T, size_t N, class U>
    struct rpred_v_impl
    {
      static void apply(tvecN<T, N>& lhs, const tvecN<U, N>& rhs)
      {
        if constexpr (Cur != End) {
          lhs.values[Cur] = Pred()(lhs.values[Cur], rhs.values[Cur]);
          rpred_v_impl<Cur + 1, End, Pred, T, N, U>::apply(lhs, rhs);
        }
      }
    };

    template <size_t Bgn, size_t End, class Pred, class T, size_t N, class U>
    void rpred_v_assign(tvecN<T, N>& lhs, const tvecN<U, N>& rhs)
    {
      static_assert(0 <= Bgn);
      static_assert(Bgn <= End);
      static_assert(End <= N);
      rpred_v_impl<Bgn, End, Pred, T, N, U>::apply(lhs, rhs);
    }
  } // namespace detail
} // namespace mocabe::nurbs