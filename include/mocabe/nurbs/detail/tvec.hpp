//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#pragma once

#include "access.hpp"
#include "arithmetic.hpp"

#include <functional>

namespace mocabe::nurbs {

  template <class T, size_t N>
  struct tvecN
  {
    T values[N];

    tvecN(const tvecN&) = default;
    tvecN(tvecN&&)      = default;
    tvecN& operator=(const tvecN&) = default;
    tvecN& operator=(tvecN&&) = default;

    tvecN()
    {
      std::uninitialized_value_construct_n(values, N);
    }

    tvecN(const T& v)
    {
      std::uninitialized_fill_n(values, N, v);
    }

    const T& operator[](size_t n) const noexcept
    {
      return values[n];
    }

    T& operator[](size_t n) noexcept
    {
      return values[n];
    }

    template <class U>
    tvecN& operator+=(const U& u)
    {
      detail::rpred_s_assign<0, N, std::plus<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator+=(const tvecN<U, N>& u)
    {
      detail::rpred_v_assign<0, N, std::plus<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator-=(const U& u)
    {
      detail::rpred_s_assign<0, N, std::minus<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator-=(const tvecN<U, N>& u)
    {
      detail::rpred_v_assign<0, N, std::minus<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator*=(const U& u)
    {
      detail::rpred_s_assign<0, N, std::multiplies<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator*=(const tvecN<U, N>& u)
    {
      detail::rpred_v_assign<0, N, std::multiplies<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator/=(const U& u)
    {
      detail::rpred_s_assign<0, N, std::divides<void>>(*this, u);
      return *this;
    }

    template <class U>
    tvecN& operator/=(const tvecN<U, N>& u)
    {
      detail::rpred_v_assign<0, N, std::divides<void>>(*this, u);
      return *this;
    }
  };

  template <class T, class U, size_t N>
  tvecN<T, N> operator+(const tvecN<T, N>& lhs, const tvecN<U, N>& rhs)
  {
    auto tmp = lhs;
    tmp += rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator+(const tvecN<T, N>& lhs, const U& rhs)
  {
    auto tmp = lhs;
    tmp += rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator-(const tvecN<T, N>& lhs, const tvecN<U, N>& rhs)
  {
    auto tmp = lhs;
    tmp -= rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator-(const tvecN<T, N>& lhs, const U& rhs)
  {
    auto tmp = lhs;
    tmp -= rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator*(const tvecN<T, N>& lhs, const tvecN<U, N>& rhs)
  {
    auto tmp = lhs;
    tmp *= rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator*(const tvecN<T, N>& lhs, const U& rhs)
  {
    auto tmp = lhs;
    tmp *= rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator/(const tvecN<T, N>& lhs, const tvecN<U, N>& rhs)
  {
    auto tmp = lhs;
    tmp /= rhs;
    return tmp;
  }

  template <class T, class U, size_t N>
  tvecN<T, N> operator/(const tvecN<T, N>& lhs, const U& rhs)
  {
    auto tmp = lhs;
    tmp /= rhs;
    return tmp;
  }

  /// 2D vector
  template <class T>
  using tvec2 = tvecN<T, 2>;

  /// 3D vector
  template <class T>
  using tvec3 = tvecN<T, 3>;

  /// 4D vector
  template <class T>
  using tvec4 = tvecN<T, 4>;


#define MOCABE_NURBS_DECL_TVEC_ACCESS(TYPE, DIM)       \
  template <class T>                                   \
  struct vector_access<TYPE<T>, DIM>                   \
  {                                                    \
    static constexpr const auto& get(const TYPE<T>& v) \
    {                                                  \
      return v[DIM];                                   \
    }                                                  \
                                                       \
    static constexpr auto& get(TYPE<T>& v)             \
    {                                                  \
      return v[DIM];                                   \
    }                                                  \
  }

  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec2, 0);
  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec2, 1);

  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec3, 0);
  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec3, 1);
  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec3, 2);

  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec4, 0);
  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec4, 1);
  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec4, 2);
  MOCABE_NURBS_DECL_TVEC_ACCESS(tvec4, 3);

#undef MOCABE_NURBS_DECL_TVEC_ACCESS

} // namespace mocabe::nurbs