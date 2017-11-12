#pragma once

#include <vector>
#include <cassert>
#include <array>

namespace nurbs {

/**
 * @fun
 * @brief create uniformed knot vectors
 * @param numPoints a number of points
 * @param degree degree
 * @return created knot vectors
 */
template <typename T = double>
std::vector<T> CreateUniformKnots(size_t numPoints, size_t degree) {
  static_assert(std::is_floating_point<T>::value,
                "invalid type (should be floating point)");

  assert (numPoints >= 1);
  assert (degree >= 0);
  assert (degree <= numPoints - 1);

  const auto n = degree + 1 + numPoints;
  std::vector<T> ret{};
  ret.reserve(n);

  assert(n >= 2);

  auto itr_n = n - 2;

  // ignored when n==2
  auto delta = (T)1 / (n - 1);

  ret.emplace_back(0);
  auto d = delta;
  for (size_t i = 0; i < itr_n; ++i) {
    ret.emplace_back(d);
    d += delta;
  }
  ret.emplace_back(1);

  return ret;
}

/**
 * @fun
 * @brief create periodic(clamped uniform) knot
 * @param numPoints number of control points
 * @param degree degree
 * @return created knots vectors
 */
template <typename T = double>
std::vector<T> CreateClampedKnots(size_t numPoints, size_t degree) {
  static_assert(std::is_floating_point<T>::value,
                "invalid type (should be floating point)");

  assert (numPoints >= 1);
  assert (degree >= 0);
  assert (degree <= numPoints - 1);

  std::vector<T> ret{};
  ret.reserve(numPoints + degree + 1);

  auto order = degree + 1;
  // number of uniform (not clamped) knots
  auto unfrm = numPoints - order;
  auto delta = (T)1 / (unfrm + 1);

  for (size_t i = 0; i < order; ++i) ret.emplace_back(0);
  auto d = delta;
  for (size_t i = 0; i < unfrm; ++i) {
    ret.emplace_back(d);
    d += delta;
  }
  for (size_t i = 0; i < order; ++i) ret.emplace_back(1);

  return ret;
}
}