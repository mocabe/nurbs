#pragma once

#include "NURBS.hpp"
namespace nurbs {
template <class T, class K>
template <class EvalTag>
typename NURBS<T, K>::point_type NURBS<T, K>::tangent(knot_type t, point_type* pos) const{
  using namespace tags;
  using namespace std;

  t = std::clamp(t, knot_type(0), knot_type(1));
  auto range = knot_range();
  t = range.first + t * (range.second - range.first);

  std::vector<wpoint_type> buffer;
  point_type tan;

  if (pos)
    *pos = knot_evaluate_DeBoor<EvalTag, true>(t, buffer, &tan);
  else
    knot_evaluate_DeBoor<EvalTag, true>(t, buffer, &tan);

  return tan;
}
} // namespace nurbs