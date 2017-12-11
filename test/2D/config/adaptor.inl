
namespace nurbs {
template <> struct access<vec2, 0> {
  static constexpr double &get(vec2 &v) { return v.x; }
  static constexpr const double &get(const vec2 &v) { return v.x; }
};
template <> struct access<vec2, 1> {
  static constexpr double &get(vec2 &v) { return v.y; }
  static constexpr const double &get(const vec2 &v) { return v.y; }
};
template <> struct access<vec3, 0> {
  static constexpr double &get(vec3 &v) { return v.x; }
  static constexpr const double &get(const vec3 &v) { return v.x; }
};
template <> struct access<vec3, 1> {
  static constexpr double &get(vec3 &v) { return v.y; }
  static constexpr const double &get(const vec3 &v) { return v.y; }
};
template <> struct access<vec3, 2> {
  static constexpr double &get(vec3 &v) { return v.z; }
  static constexpr const double &get(const vec3 &v) { return v.z; }
};

template <> struct dimension<vec3> { static constexpr size_t value = 3; };
template <> struct dimension<vec2> { static constexpr size_t value = 2; };

template <> struct point_traits<vec3> {
  using point_type = vec2;
  using weighted_point_type = vec3;
};
}