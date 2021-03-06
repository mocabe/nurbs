#pragma once

#include "NURBS.hpp"
#include <utility>

namespace nurbs {
/**
 * @fun
 * @brief reverse NURBS segment.
 */
template <class T, class K>
NURBS<T, K>& NURBS<T, K>::reverse() {
  /*
    1. Init new knot vector which has same size to current knots.
    2. Set a base knot.
    3. diff(i) = knots[i] - knots[i-1]
    4. new_knots[j+1] = new_knots[j] + diff(j)
  */
  auto& nurbs = *this;

  std::reverse(nurbs.pbegin(), nurbs.pend());
  std::vector<typename NURBS<T, K>::knot_type> new_knots(nurbs.ksize());

  new_knots[0] = nurbs.kget(0);

  auto ksize = nurbs.ksize();
  for (size_t i = ksize - 1; 0 < i; --i) {
    auto diff = nurbs.kget(i) - nurbs.kget(i - 1);
    size_t index = ksize - i;
    new_knots[index] = new_knots[index - 1] + diff;
  }
  nurbs.set_knots(std::move(new_knots));

  return nurbs;
}
} // namespace nurbs