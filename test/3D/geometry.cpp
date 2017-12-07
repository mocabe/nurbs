#include <src/detail/NURBS.hpp>
#include <src/util/nurbs_util.hpp>
#include "config/adaptor.hpp"
#include "../test.hpp"
using namespace nurbs;

void dot(){
  dvec3 p1 {2,1,-5};
  dvec3 p2 {3,-1,1};
  t_assert("dot test", nurbs::dot(p1, p2) == 0);
}
void cross() {
  dvec3 p1 {1,0,0};
  dvec3 p2 {0,1,0};
  dvec3 p3 {0,0,1};
  t_assert("cross test",nurbs::cross(p1, p2) == p3);
}
void distance(){
  dvec3 p1 {1,0,0};
  dvec3 p2 {-2,0,4};
  t_assert("distance test", nurbs::distance(p1, p2) == 5.);
}

void normalize() {
  dvec3 p1 {-1,2,-2};
  t_assert("normalize ", nurbs::normalize(p1) == p1 / 3 );

  dvec3 p2 {0,0,0};
  t_assert("normalize zero vector", nurbs::normalize(p2) == p2);
}
int main(){
  test::test_name = "geometric function test";
  dot();
  cross();
  distance();
  normalize();
  test::summarize();
  return test::test_failed;
}