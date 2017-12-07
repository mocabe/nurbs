#pragma once

#include "NURBS.hpp"

namespace nurbs {
  template <class T, class K>
  NURBS<T,K>& NURBS<T,K>::knot_insert(knot_type t){
    /*
      1. find knot span index 'k';
         points[k-degree] to points[k] will be use
      2. q[i] = (1- a(i))p[i-1] + a(i)p[i] 
           where a(i) = (t - k[i]() / (k[i+degree] - k[i]), i= k-degree+1 to k
      3. replace p[k-degree+1] to p[k-1] with q[k-regree+1] to q[k] 
         insert new knot
      
      Note: calculating p[i] requires p[i] and p[i-1] on this formula. 
            so here we simply iterate 'i' in reverse order. (from k to k-degree+1)
    */

    auto range = knot_range();
    t = std::clamp(t, range.first, range.second);

    size_t index;
    // find knot span
    for (index = 0; index < knots_.size() - 2; ++index)
      if (knots_[index] <= t && t < knots_[index + 1])
        break;

    while (knots_[index] == knots_[index + 1]) {
      if(index == 0) break;
      --index;
    }

    size_t min_p_index = (index < degree_) ? 0 : index - degree_;
    size_t max_p_index = (points_.size() <= index) ? points_.size() - 1 : index;

    // NURBS -> BSpline
    for (size_t i = min_p_index; i <= max_p_index; ++i)
      nurbs::rmult<dimension_v<point_type> - 1>(
          points_[i], get<dimension_v<point_type>>(points_[i]));

    size_t min_q_index = (index + 1 < degree_) ?  0 : index + 1 - degree_;
    size_t max_q_index = (points_.size() <= index) ? points_.size() - 1 : index;

    // degree == 0
    if (min_q_index > max_q_index)
      max_q_index = min_q_index;

    // insert points
    for (size_t i = max_q_index + 1; i-- > min_q_index;) {
      auto d = knots_[i + degree_] - knots_[i];
      auto a = (d == 0) ? 0 : (t - knots_[i]) / d;
      wpoint_type q;
      if (i == 0)
        q = points_[i];
      else if (i == points_.size())
        q = points_[i - 1];
      else {
        if (get<dimension_v<point_type>>(points_[i - 1]) == 0) {
          q = points_[i] * a;
          if (get<dimension_v<point_type>>(q) == 0)
            q = points_[i - 1];
        } else if (get<dimension_v<point_type>>(points_[i]) == 0) {
          q = points_[i - 1] * (1 - a);
          if (get<dimension_v<point_type>>(q) == 0)
            q = points_[i];
        } else
          q = points_[i - 1] * (1 - a) + points_[i] * a;
      }

      if (i == max_q_index)
        points_.emplace(points_.begin() + i, q);
      else
        points_[i] = q;
    }

    // BSpline -> NURBS
    for (size_t i = min_p_index; i <= max_p_index + 1 ; ++i)
      if (get<dimension_v<point_type>>(points_[i]) != 0)
        nurbs::rdiv<dimension_v<point_type> - 1>(
            points_[i], get<dimension_v<point_type>>(points_[i]));

    // insert knot
    knots_.insert(knots_.begin() + index + 1, t);

    return *this;
  }
}