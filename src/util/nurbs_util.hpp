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

  if (numPoints == 0)
    throw std::invalid_argument("numPoints==0");
  if(degree > numPoints -1)
    throw std::invalid_argument("degree <= numPoints -1");

  const auto n = degree + 1 + numPoints;
  std::vector<T> ret {};
  ret.reserve(n);

  for (size_t i = 0; i < n; ++i) {
    ret.emplace_back((T)i / (n - 1));
  }

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

  if (numPoints == 0)
    throw std::invalid_argument("numPoints==0");
  if(degree > numPoints -1)
    throw std::invalid_argument("degree <= numPoints -1");

  const auto n = degree + 1 + numPoints;
  std::vector<T> ret{};
  ret.reserve(n);

  // number of uniform (not clamped) knots
  const auto unfrm = n - 2 * degree;

  for (size_t i = 0; i < degree; ++i) ret.emplace_back(0);

  for (size_t i = 0; i < unfrm; ++i) {
    ret.emplace_back((T)i / (unfrm - 1));
  }

  for (size_t i = 0; i < degree; ++i) ret.emplace_back(1);

  return ret;
}
}