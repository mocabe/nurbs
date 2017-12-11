
namespace nurbs {
template <> struct access<vec3, 0> {
  static auto &get(vec3 &v) { return v.x; }
  static const auto &get(const vec3 &v) { return v.x; }
};
template <> struct access<vec3, 1> {
  static auto &get(vec3 &v) { return v.y; }
  static const auto &get(const vec3 &v) { return v.y; }
};
template <> struct access<vec3, 2> {
  static auto &get(vec3 &v) { return v.z; }
  static const auto &get(const vec3 &v) { return v.z; }
};
template <> struct access<vec4, 0> {
  static auto &get(vec4 &v) { return v.x; }
  static const auto &get(const vec4 &v) { return v.x; }
};
template <> struct access<vec4, 1> {
  static auto &get(vec4 &v) { return v.y; }
  static const auto &get(const vec4 &v) { return v.y; }
};
template <> struct access<vec4, 2> {
  static auto &get(vec4 &v) { return v.z; }
  static const auto &get(const vec4 &v) { return v.z; }
};
template <> struct access<vec4, 3> {
  static auto &get(vec4 &v) { return v.w; }
  static const auto &get(const vec4 &v) { return v.w; }
};
template <> struct dimension<vec3> { static constexpr size_t value = 3; };
template <> struct dimension<vec4> { static constexpr size_t value = 4; };

template <> struct point_traits<vec4> {
  using point_type = vec3;
  using weighted_point_type = vec4;
};
}