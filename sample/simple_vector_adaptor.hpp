#pragma once

namespace smplv {
struct dvec3 {
  double x,y,z;
};
struct dvec4 {
  double x,y,z,w;
};
}
namespace nurbs {
  using namespace smplv;
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

template <> struct dimension<dvec4> { static const int value = 4; };
template <> struct dimension<dvec3> { static const int value = 3; };

template <> struct point_traits<dvec3> {
  using point_type = dvec3;
  using weighted_point_type = dvec4;
};
}