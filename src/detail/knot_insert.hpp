#pragma once

#include <nurbs.hpp>

namespace nurbs {
  template <class T, class K>
  void NURBS<T,K>::knot_insert(knot_type t){
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
    t = std::max(range.first, std::min(t, range.second));

    size_t index;
    // find knot span
    for (index = degree_; index < points_.size() - 1; ++index) {
      if (knots_[index] <= t && t < knots_[index + 1])
        break;
    }

    // convert NURBS to BSpline
    for (size_t i = index - degree_; i <= index; ++i) {
      rmult<dimension_v<point_type> - 1>(points_[i],
                                         get<dimension_v<point_type>>(points_[i]));
    }

    // update points
    for (size_t i = index; i >= index - degree_ + 1; --i) {
      knot_type a = (knots_[i + degree_] != knots_[i])
                   ? (t - knots_[i]) / (knots_[i + degree_] - knots_[i])
                   : 0;
      wpoint_type q = points_[i - 1] - (points_[i - 1] - points_[i]) * a;

      if (i != index){
        points_[i] = q;
      } else {
        points_.emplace(points_.begin() + i, q);
      }
    }

    // convert BSpline to NURBS
    for (size_t i = index - degree_; i <= index + 1; ++i) {
      rdiv<dimension_v<point_type> - 1>(
          points_[i], get<dimension_v<point_type>>(points_[i]));
    }

    // update knot
    knots_.insert(knots_.begin() + index + 1, t);
  }
}