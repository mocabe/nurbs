#pragma once
#include <glm/glm.hpp>

namespace nurbs {
using namespace glm;
template <> struct access<dvec3, 0> {
  static double &get(dvec3 &v) { return v[0]; }
  static const double &get(const dvec3 &v) { return v[0]; }
};
template <> struct access<dvec3, 1> {
  static double &get(dvec3 &v) { return v[1]; }
  static const double &get(const dvec3 &v) { return v[1]; }
};
template <> struct access<dvec3, 2> {
  static double &get(dvec3 &v) { return v[2]; }
  static const double &get(const dvec3 &v) { return v[2]; }
};
template <> struct access<dvec4, 0> {
  static double &get(dvec4 &v) { return v[0]; }
  static const double &get(const dvec4 &v) { return v[0]; }
};
template <> struct access<dvec4, 1> {
  static double &get(dvec4 &v) { return v[1]; }
  static const double &get(const dvec4 &v) { return v[1]; }
};
template <> struct access<dvec4, 2> {
  static double &get(dvec4 &v) { return v[2]; }
  static const double &get(const dvec4 &v) { return v[2]; }
};
template <> struct access<dvec4, 3> {
  static double &get(dvec4 &v) { return v[3]; }
  static const double &get(const dvec4 &v) { return v[3]; }
};

template <> struct dimension<dvec4> { static const int value = 4; };
template <> struct dimension<dvec3> { static const int value = 3; };

template <> struct point_traits<dvec3> {
  using point_type = dvec3;
  using weighted_point_type = dvec4;
};
}