#pragma once

namespace nurbs{
template <class T>
struct point_traits{
  using point_type = T;
  using weighted_point_type = typename T::weighted_point_type;
};
}