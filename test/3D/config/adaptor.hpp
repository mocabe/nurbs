#pragma once

#include "struct.hpp"

namespace nurbs {
template <> struct access<dvec3, 0> {
  static double &get(dvec3 &v) { return v.x; }
  static const double &get(const dvec3 &v) { return v.x; }
};
template <> struct access<dvec3, 1> {
  static double &get(dvec3 &v) { return v.y; }
  static const double &get(const dvec3 &v) { return v.y; }
};
template <> struct access<dvec3, 2> {
  static double &get(dvec3 &v) { return v.z; }
  static const double &get(const dvec3 &v) { return v.z; }
};
template <> struct access<dvec4, 0> {
  static double &get(dvec4 &v) { return v.x; }
  static const double &get(const dvec4 &v) { return v.x; }
};
template <> struct access<dvec4, 1> {
  static double &get(dvec4 &v) { return v.y; }
  static const double &get(const dvec4 &v) { return v.y; }
};
template <> struct access<dvec4, 2> {
  static double &get(dvec4 &v) { return v.z; }
  static const double &get(const dvec4 &v) { return v.z; }
};
template <> struct access<dvec4, 3> {
  static double &get(dvec4 &v) { return v.w; }
  static const double &get(const dvec4 &v) { return v.w; }
};
template <> struct dimension<dvec3> { static constexpr size_t value = 3; };
template <> struct dimension<dvec4> { static constexpr size_t value = 4; };

template <> struct point_traits<dvec3> {
  using point_type = dvec3;
  using weighted_point_type = dvec4;
};
}