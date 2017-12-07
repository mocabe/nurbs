#include <src/detail/split.hpp>
#include <src/util/nurbs_util.hpp>
#include "config/adaptor.hpp"
#include "../test.hpp"
#include <iostream>

using namespace nurbs;
void split() {
    size_t degree = 2;
    std::vector<dvec3> points = {
        {0, 0, 1}, {1, 1, 0.5}, {2, 1, 2}, {3, 0, 1}};
    auto knots = CreateClampedKnots(points.size(), degree);
    NURBS<dvec3, double> nurbs{points, knots, degree};
    {
      auto[c1, c2] = nurbs.split(0.5);
      t_assert("split continuity test1",
               c1.points().back() == c2.points().front());
    }
    {
      auto[c1, c2] = nurbs.split(0);
      t_assert("split continuity test2",
               c1.points().back() == c2.points().front());
    }
    {
      auto[c1, c2] = nurbs.split(1);
      t_assert("split continuity test3",
               c1.points().back() == c2.points().front());
    }
}
int main(){
  test::test_name = "split test";
  split();
  test::summarize();
  return test::test_failed;
}