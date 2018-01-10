#include "../test.hpp"
#include <src/detail/intersec.hpp>
#include <src/util/nurbs_util.hpp>
#include "config/adaptor.hpp"
#include <iostream>
#include <cmath>

using namespace nurbs;

void intersec() {
  size_t degree = 2;
  {
    std::vector<dvec3> points = {{0, 0, 1}, {2, 4, 1}, {4, -4, 1}, {6, 0, 1}};
    auto knots = CreateClampedKnots(points.size(), degree);
    NURBS<dvec3, double> nurbs{points, knots, degree};
    dvec2 orig{0, 0};
    dvec2 dir{1, 0};
    std::vector<dvec2> result;
    nurbs.intersec(orig, dir, result);
    std::vector<dvec2> expected{{0, 0}, {3, 0}, {6, 0}};
    std::cout << "Intersec:" << std::endl;
    for(auto&& r : result){
      std::cout << "{" << r.x << "," << r.y << "}" << std::endl;
    }
    std::cout << std::endl;
    bool b = true;
    double err = 1e-5;
    for (auto&& e : expected) {
      bool bb = false;
      for(auto && r :result){
        if (std::abs(e.x - r.x) < err && std::abs(e.y - r.y) < err) {
          bb = true;
          break;
        }
      }
      b = b && bb;
    }
    t_assert("intersec test 1", b);
  }
}
int main() {
  test::test_name = "intersec test";
  intersec();
  test::summarize();
  return test::test_failed;

}