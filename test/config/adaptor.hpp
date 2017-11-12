#pragma once

#include "struct.hpp"

namespace nurbs {
template <> struct access<dvec2, 0> {
  static double &get(dvec2 &v) { return v.x; }
  static const double &get(const dvec2 &v) { return v.x; }
};
template <> struct access<dvec2, 1> {
  static double &get(dvec2 &v) { return v.y; }
  static const double &get(const dvec2 &v) { return v.y; }
};
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

template <> struct dimension<dvec3> { static const int value = 3; };
template <> struct dimension<dvec2> { static const int value = 2; };

template <> struct point_traits<dvec2> {
  using point_type = dvec2;
  using weighted_point_type = dvec3;
};
}