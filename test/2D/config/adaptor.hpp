#pragma once

#include "struct.hpp"

namespace nurbs {
template <> struct access<dvec2, 0> {
  static constexpr double &get(dvec2 &v) { return v.x; }
  static constexpr const double &get(const dvec2 &v) { return v.x; }
};
template <> struct access<dvec2, 1> {
  static constexpr double &get(dvec2 &v) { return v.y; }
  static constexpr const double &get(const dvec2 &v) { return v.y; }
};
template <> struct access<dvec3, 0> {
  static constexpr double &get(dvec3 &v) { return v.x; }
  static constexpr const double &get(const dvec3 &v) { return v.x; }
};
template <> struct access<dvec3, 1> {
  static constexpr double &get(dvec3 &v) { return v.y; }
  static constexpr const double &get(const dvec3 &v) { return v.y; }
};
template <> struct access<dvec3, 2> {
  static constexpr double &get(dvec3 &v) { return v.z; }
  static constexpr const double &get(const dvec3 &v) { return v.z; }
};

template <> struct dimension<dvec3> { static constexpr size_t value = 3; };
template <> struct dimension<dvec2> { static constexpr size_t value = 2; };

template <> struct point_traits<dvec3> {
  using point_type = dvec2;
  using weighted_point_type = dvec3;
};
}