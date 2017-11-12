#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include <nurbs.hpp>
#include <test/test.hpp>
#include <test/2D/config/adaptor.hpp>
using namespace nurbs;

void util(){
  {
    auto knots = CreateClampedKnots(6,2);
    std::vector<double> a = {0, 0, 0, 0.25, 0.5, 0.75, 1, 1, 1};
    t_assert("clamped", knots == a);
  }
  {
    auto knots = CreateUniformKnots(6,2);
    std::vector<double> a = {0, 1./8, 2./8, 3./8, 4./8, 5./8, 6./8, 7./8, 1};
    t_assert("uniform", knots == a);
  }
}
void evaluate_edge(){
  size_t degree =2;
  std::vector<dvec3> points = {
    {1,1,1},
    {2,2,1},
    {3,1,1}
  };
  auto knots = CreateClampedKnots(points.size(),degree);
  NURBS<dvec2,double> nurbs{points,knots,degree};
  { 
    auto r = nurbs.evaluate(0); 
    t_test("evaluate(0)",r.x==points.front().x && r.y==points.front().y);
  }
  { 
    auto r = nurbs.evaluate(1); 
    t_test("evaluate(1)",r.x==points.back().x && r.y==points.back().y);
  }
  {
    t_assert("clamp", nurbs.evaluate(1) == nurbs.evaluate(2));
    t_assert("clamp", nurbs.evaluate(0) == nurbs.evaluate(-1));
  }
}

void evaluate_all_edge(){
  size_t degree =2;
  std::vector<dvec3> points = {
    {1,1,1},
    {2,2,1},
    {3,1,1}
  };
  auto knots = CreateClampedKnots(points.size(),degree);
  NURBS<dvec2,double> nurbs{points,knots,degree};
  {
    auto r= nurbs.evaluate_all(-1);
    t_assert("size()==2",r.size()==2);
    t_test("evaluate_all(-1) should return {points.back(),points.front()}",
      r.front().x == points.back().x && r.front().y == points.back().y &&
      r.back().x == points.front().x && r.back().y == points.front().y);
  }
  {
    auto r= nurbs.evaluate_all(0);
    t_assert("size()==2",r.size()==2);
    t_test("evaluate_all(0) should return {points.front(),points.front()}",
      r.front() == r.back() && 
      r.back().x == points.front().x && r.back().y == points.front().y);
  }
  {
    auto r= nurbs.evaluate_all(1);
    t_test("size()==2",r.size()==2);
    t_assert("evaluate_all(1) should return {points.front(),points.back()}",
      r.front().x == points.front().x && r.front().y == points.front().y &&
      r.back().x == points.back().x && r.back().y == points.back().y);
  }
}
void evaluate_all_size(){
  size_t degree =2;
  std::vector<dvec3> points = {
    {1,1,1},
    {2,2,1},
    {3,1,1}
  };
  auto knots = CreateClampedKnots(points.size(),degree);
  NURBS<dvec2,double> nurbs{points,knots,degree};
  {
    auto r= nurbs.evaluate_all(0.1);
    t_assert("size", r.size() == 12);
  }
}

void evaluate_zero_weight() {
  size_t degree = 2;
  std::vector<dvec3> points(3);
  auto knots = CreateClampedKnots(points.size(), degree);
  NURBS<dvec2, double> nurbs{points, knots, degree};
  {
    std::vector<dvec3> p = {{1, 1, 0}, {2, 2, 1}, {3, 1, 0}};
    std::copy(p.begin(), p.end(), nurbs.pbegin());
    auto r = nurbs.evaluate(1);
    std::stringstream m;
    m << "zero weight test: r{" << r.x << "," << r.y << "} should be {"
      << p[1].x << "," << p[1].y << "}";
    t_assert(m.str(), r == degenerate<dvec2>(p[1]));
  }

  {
    std::vector<dvec3> p = {{1, 1, 0}, {2, 2, 1}, {3, 1, 1}};
    std::copy(p.begin(), p.end(), nurbs.pbegin());
    auto r = nurbs.evaluate(0);
    t_assert("zero weight test edge-front", r == degenerate<dvec2>(p[1]));
  }
  {
    std::vector<dvec3> p = {{1, 1, 1}, {2, 2, 1}, {3, 1, 0}};
    std::copy(p.begin(), p.end(), nurbs.pbegin());
    auto r = nurbs.evaluate(1);
    t_assert("zero weight test edge-back", r == degenerate<dvec2>(p[1]));
  }

  points.resize(5);
  knots = CreateClampedKnots(points.size(), degree);
  nurbs = {points, knots, degree};
  { 
    std::vector<dvec3> p = {{1, 1, 1}, {2, 2, 0}, {3, 1, 0}, {4, -1, 1}, {5, 0, 1}}; 
    std::copy(p.begin(), p.end(), nurbs.pbegin());
    auto r=nurbs.evaluate(1./3);
    t_assert("zero weight test mid x2", r==degenerate<dvec2>(p[3]));
  }
  {
    std::vector<dvec3> p = {{1, 1, 1}, {2, 2, 0}, {3, 1, 0}, {4, -1, 0}, {5, 0, 1}}; 
    std::copy(p.begin(), p.end(), nurbs.pbegin());
    auto r=nurbs.evaluate(1./3);
    t_assert("zero weight test mid x3", r==degenerate<dvec2>(p[4]));
    r=nurbs.evaluate(2./3);
    t_assert("zero weight test mid x3", r==degenerate<dvec2>(p[4]));
  }
}

int main(){
  util();
  evaluate_edge();
  evaluate_all_edge();
  evaluate_all_size();
  evaluate_zero_weight();
  nurbs::test::summarize();
  return nurbs::test::test_failed;
}