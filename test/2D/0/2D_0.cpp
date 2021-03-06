#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include <nurbs.hpp>
#include <test/test.hpp>
#include <test/2D/config/adaptor.hpp>

using namespace nurbs;

void default_ctor(){
  NURBS<dvec3,double> nurbs{};
  t_assert("default constructor should generate 1-point, 0-degree curve",nurbs.degree()==0 && nurbs.psize()== 1);
}

void ctor(){
  size_t degree =0;
  std::vector<dvec3> points = {{1, 1, 1}};
  auto knots = CreateClampedKnots(points.size(),degree);
  NURBS<dvec3, double> nurbs{points, knots,degree};
  t_assert("0 degree ctor", nurbs.degree()==0 && nurbs.psize()==1 && nurbs.points().front() == points.front());
}

void eval(){
  size_t degree =0;
  std::vector<dvec3> points = {{1, 1, 1}};
  auto knots = CreateClampedKnots(points.size(),degree);
  NURBS<dvec3, double> nurbs{points, knots,degree};
  {
    auto r = nurbs.evaluate_all(0.1);
    for( auto p: r){
      t_assert("eval test 0", p == degenerate<dvec2>(points[0]));
    }
  }
}
int main(){
  test::test_name = "0 degree tests";
  default_ctor();
  ctor();
  eval();
  nurbs::test::summarize();
  return nurbs::test::test_failed;
}