#pragma once

#include <nurbs.hpp>

#include <utility>

namespace nurbs {
  template <class T, class K>
  void reverse(NURBS<T,K>& nurbs){
    std::reverse(nurbs.pbegin(), nurbs.pend());
    std::vector<typename NURBS<T,K>::knot_type> new_knots(nurbs.ksize());

    new_knots[0] = nurbs.kget(0);

    auto ksize = nurbs.ksize();
    for (size_t i = ksize - 1; 0 < i; --i) {
      auto diff = nurbs.kget(i) - nurbs.kget(i - 1);
      size_t index = ksize - i;
      new_knots[index] = new_knots[index - 1] + diff;
    }
    nurbs.set_knots(std::move(new_knots));
  }
}