#pragma once
#include <glm/glm.hpp>

namespace nurbs {
using namespace glm;
template <> struct access<dvec3, 0> {
  static constexpr double &get(dvec3 &v) { return v[0]; }
  static constexpr const double &get(const dvec3 &v) { return v[0]; }
};
template <> struct access<dvec3, 1> {
  static constexpr double &get(dvec3 &v) { return v[1]; }
  static constexpr const double &get(const dvec3 &v) { return v[1]; }
};
template <> struct access<dvec3, 2> {
  static constexpr double &get(dvec3 &v) { return v[2]; }
  static constexpr const double &get(const dvec3 &v) { return v[2]; }
};
template <> struct access<dvec4, 0> {
  static constexpr double &get(dvec4 &v) { return v[0]; }
  static constexpr const double &get(const dvec4 &v) { return v[0]; }
};
template <> struct access<dvec4, 1> {
  static constexpr double &get(dvec4 &v) { return v[1]; }
  static constexpr const double &get(const dvec4 &v) { return v[1]; }
};
template <> struct access<dvec4, 2> {
  static constexpr double &get(dvec4 &v) { return v[2]; }
  static constexpr const double &get(const dvec4 &v) { return v[2]; }
};
template <> struct access<dvec4, 3> {
  static constexpr double &get(dvec4 &v) { return v[3]; }
  static constexpr const double &get(const dvec4 &v) { return v[3]; }
};

template <> struct dimension<dvec4> { static constexpr size_t value = 4; };
template <> struct dimension<dvec3> { static constexpr size_t value = 3; };

template <> struct point_traits<dvec4> {
  using point_type = dvec3;
  using weighted_point_type = dvec4;
};
}