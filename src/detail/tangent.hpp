#pragma once

#include "NURBS.hpp"
namespace nurbs {
template <class T, class K>
typename NURBS<T, K>::point_type NURBS<T, K>::tangent(knot_type t) const{
  using namespace tags;
  using namespace std;

  t = std::clamp(t, knot_type(0), knot_type(1));
  auto range = knot_range();
  t = range.first + t * (range.second - range.first);

  size_t index;
  // find knot span
  for (index = 0; index < knots_.size() - 2; ++index) {
    if (knots_[index] <= t && t < knots_[index + 1])
      break;
  }

  while (knots_[index] == knots_[index + 1]) {
    if (index == 0)
      break;
    --index;
  }
  assert(knots_.size() > index);

  // return vector
  wpoint_type r;

  // buffer
  std::vector<wpoint_type> buff(degree_ + 1);

  for (size_t i = 0; i < degree_ + 1; ++i) {
    // invalid index
    if (index + i < degree_ || index - degree_ + i >= points_.size()) {
      buff[i] = wpoint_type{} * 0;
    } else {
      buff[i] = points_[index - degree_ + i];
      rmult<dimension_v<point_type> - 1>(buff[i],
                                         get<dimension_v<point_type>>(buff[i]));
    }
  }

  // i=0 to degree-2
  for (size_t i = 1; i-- < degree_; i += 2) {
    for (size_t j = 0; j < degree_ - i; ++j) {
      size_t idx = index - degree_ + i + j + 1;
      // invalid index
      if (idx >= knots_.size() ||
          idx + degree_ + 1 - (i + 1) >= knots_.size()) {
        buff[j] = wpoint_type{} * 0;
      } else {
        knot_type d = knots_[idx + degree_ + 1 - (i + 1)] - knots_[idx];
        knot_type a = (d == 0) ? 0 : (t - knots_[idx]) / d;
        buff[j] = buff[j] - (buff[j] - buff[j + 1]) * a;
      }
    }
  }

  wpoint_type r1 = buff[1];
  wpoint_type r2 = buff[0];
  auto d = (knot_type)degree_ / (knots_[index + 1] - knots_[index]);
  r = (d != 0) ? (r1 * get<dimension_v<point_type>>(r2) -
                  r2 * get<dimension_v<point_type>>(r1)) *
                     d
               : wpoint_type{} * 0;

  // return as point_type
  return degenerate<point_type>(r);
}
} // namespace nurbs