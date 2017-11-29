#include <nurbs.hpp>
#include "config/adaptor.hpp"
#include "../test.hpp"
#include <iostream>
#include <cmath>
using namespace nurbs;
void tangent(){
  {
    size_t degree = 2;
    std::vector<dvec3> points = {
        {0, 0, 1}, {1, 1, 0.5}, {2, 1, 2}, {3, 0, 1}};
    auto knots = CreateClampedKnots(points.size(), degree);
    NURBS<dvec3, double> nurbs{points, knots, degree};
    double interval = 0.0001;
    double err = 0;
    auto r = nurbs.evaluate_all(interval);
    for (auto i = 1; i < r.size()-1; ++i) {
      auto t1 = r[i+1] - r[i-1];
      auto t2 = nurbs.tangent(interval*(i-1));
      auto cross = t1.x * t2.y - t1.y * t2.x;
      err = std::max(err, cross);
    }
    std::cout<< "err:" << err << std::endl;
    t_assert("tangent error test ", err < 1e-6);
  }
}
int main(){
  test::test_name = "tangent test";
  tangent();
  test::summarize();
  return test::test_failed;
}