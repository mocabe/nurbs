#include <src/util/vector_traits.hpp>
#include <src/util/point_traits.hpp>
#include <src/util/geometry.hpp>
#include "config/df_adaptor.hpp"

using namespace nurbs;

int main() {
  static_assert(has_single_type_v<dfvec2>);
  static_assert(!has_single_type_v<dfvec3>);
  dfvec2 sv; // all elements are double
  dfvec3 v;
  float f = 1;
  // should compile
  dot(v, v);
  fma_division(f, sv, sv, sv);
  fma_division(f, v, v, v);
}