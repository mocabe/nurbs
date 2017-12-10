#pragma once

#include "NURBS.hpp"
namespace nurbs {
template <class T, class K>
template <class EvalTag>
typename NURBS<T, K>::point_type NURBS<T, K>::tangent(
  knot_type t, point_type* pos) const {
  t = map_to_knot_range(t);

  std::vector<wpoint_type> buffer;
  point_type tan;

  if (pos)
    *pos = knot_evaluate_DeBoor<EvalTag, true>(t, buffer, &tan);
  else
    knot_evaluate_DeBoor<EvalTag, true>(t, buffer, &tan);

  return tan;
}
} // namespace nurbs