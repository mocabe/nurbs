#include <src/detail/reverse.hpp>
#include <src/util/nurbs_util.hpp>
#include "config/adaptor.hpp"
#include "../test.hpp"
#include <iostream>
#include <cmath>
using namespace nurbs;

void basic_reverse(){
  {
    size_t degree = 2;
    std::vector<dvec3> points = {
        {0, 0, 1}, {1, 1, 0.5}, {2, 1, 0.5}, {3, 0, 1}};
    auto knots = CreateClampedKnots(points.size(), degree);
    NURBS<dvec3, double> nurbs{points, knots, degree};
    auto r1 = nurbs.evaluate_all(-0.125);
    nurbs.reverse();
    auto r2 = nurbs.evaluate_all(0.125);
    std::reverse(points.begin(), points.end());
    t_assert("reverse point test", nurbs.points() == points);
    t_assert("reverse result test", r1 == r2);
  }
  {
    size_t degree = 2;
    std::vector<dvec3> points = {
        {0, 0, 1}, {1, 1, 0.5}, {2, 1, 0.5}, {3, 0, 1}};
    auto knots = CreateUniformKnots(points.size(), degree);
    NURBS<dvec3, double> nurbs{points, knots, degree};
    auto r1 = nurbs.evaluate_all(-0.125);
    nurbs.reverse();
    auto r2 = nurbs.evaluate_all(0.125);
    std::reverse(points.begin(), points.end());

    t_assert("reverse point test", nurbs.points() == points);

    bool flag = true;
    for(size_t i=0;i<std::min(r1.size(),r2.size()); ++i){
      if(std::fabs(r1[i].x - r2[i].x) >= 1e-14)
        flag = false;
      if(std::fabs(r1[i].y - r2[i].y) >= 1e-14)
        flag = false;
    }
    t_assert("reverse result test", flag);
  }
}

void edge_cases(){

}

int main(){
  test::test_name = "reverse test";
  basic_reverse();
  edge_cases();
  test::summarize();
  return test::test_failed;
}