#pragma once

#include "knot_insert.hpp"

namespace nurbs {

template <class T, class K>
std::pair<NURBS<T, K>, NURBS<T, K>> NURBS<T, K>::split(knot_type t) const && {

  t = map_to_knot_range(t);

  size_t knots_exist = 0;
  for (auto &&k : knots_) {
    if (k == t) ++knots_exist;
    if (k > t) break;
  }

  // move
  NURBS ret1{std::move(*this)};

  assert(ret1.ksize() == ret1.psize() + ret1.degree() + 1);
  size_t insert_required;
  if (knots_exist > degree_ + 1)
    insert_required = 0;
  else
    insert_required = degree_ + 1 - knots_exist;

  for (size_t i = 0; i < insert_required; ++i)
    ret1.knot_insert(t);

  knots_exist += insert_required;

  size_t index = 0;
  for (auto &&k : ret1.knots_) {
    if (k == t) break;
    ++index;
  }

  // padding head
  if (index < degree_ + 1) {
    for (size_t i = 0; i < degree_ + 1 - index; ++i)
      ret1.knot_insert(t);
    knots_exist += degree_ + 1 - index;
  }

  // padding tail
  if (ret1.knots_.size() - index < 2 * (degree_ + 1)) {
    auto n = 2 * (degree_ + 1) - (ret1.knots_.size() - index);
    for (size_t i = 0; i < n; ++i)
      ret1.knot_insert(t);
    knots_exist += n;
  }

  // build ret2
  auto last_idx = index + knots_exist - 1;
  size_t n = std::min(last_idx - degree(),ret1.knots_.size() - 2*(degree() + 1));
  
  std::vector<knot_type> ret2_knots(
    ret1.knots_.begin() + n, ret1.knots_.end());

  std::vector<wpoint_type> ret2_points(
    ret1.points_.begin() +
      (ret1.points_.size() - (ret2_knots.size() - degree_ - 1)),
    ret1.points_.end());

  NURBS ret2{std::move(ret2_points), std::move(ret2_knots), degree_};

  // shrink ret1
  n = std::max(last_idx + 1, 2*(degree() + 1));
  ret1.knots_.erase(
    ret1.knots_.begin() + n, ret1.knots_.end());

  ret1.points_.erase(
    ret1.points_.begin() + (ret1.knots_.size() - degree_ - 1),
    ret1.points_.end());

  ret1.points_.shrink_to_fit();
  ret1.knots_.shrink_to_fit();

  assert(ret1.psize() + ret1.degree() + 1 == ret1.ksize());
  assert(ret2.psize() + ret2.degree() + 1 == ret2.ksize());

  return {std::move(ret1), std::move(ret2)};
}

template <class T, class K>
std::pair<NURBS<T, K>, NURBS<T, K>> NURBS<T, K>::split(knot_type t) const & {
  NURBS cpy{*this};
  return std::move(cpy).split(t);
}
} // namespace nurbs
