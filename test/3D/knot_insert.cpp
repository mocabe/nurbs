#include <nurbs.hpp>
#include "config/adaptor.hpp"
#include "../test.hpp"
#include <iostream>
#include <cmath>

using namespace nurbs;

void ex1(){
  const size_t degree = 3;
  const std::vector<dvec4> points = {{1, 2, 1, 1}, {3, 0, 0, 1}, {5, 4, 1, 1},
                                     {7, 2, 0, 1}, {5, 8, 1, 1}, {3, 6, 0, 1},
                                     {1, 2, 1, 1}, {0, 12, 0, 1}};
  const auto knots = CreateClampedKnots(points.size(), degree);
  { // t = 0.5
    auto nurbs = NURBS<dvec3, double>(points, knots, degree);
    auto r1 = nurbs.evaluate_all(0.125);
    nurbs.knot_insert(0.5);
    std::vector<double> a = {0, 0, 0, 0, 1./5, 2./5, 0.5, 3./5, 4./5, 1, 1, 1, 1};
    t_assert("inserted knot test", nurbs.knots() == a);

    auto r2 = nurbs.evaluate_all(0.125);
    dvec3 error{0, 0, 0};
    for(size_t i=0; i< std::min(r1.size(),r2.size()); ++i){
      error.x = std::max(error.x,std::fabs(r1[i].x - r2[i].x));
      error.y = std::max(error.y,std::fabs(r1[i].y - r2[i].y));
      error.z = std::max(error.z,std::fabs(r1[i].z - r2[i].z));
    }
    std::cout << "error: ";
    std::cout << "  {" << error.x << "," << error.y << "," << error.z << "}" << std::endl;
    double tolerance = 1e-14;
    t_assert("insert result error test", error.x < tolerance && error.y < tolerance && error.z < tolerance);
  }
  { // t = 0
    auto nurbs = NURBS<dvec3, double>(points, knots, degree);
    auto r1 = nurbs.evaluate_all(0.125);
    nurbs.knot_insert(0);
    std::vector<double> a = {0, 0, 0, 0, 0, 1./5, 2./5,3./5, 4./5, 1, 1, 1, 1};
    t_assert("inserted knot test", nurbs.knots() == a);

    auto r2 = nurbs.evaluate_all(0.125);
    dvec3 error{0, 0, 0};
    for(size_t i=0; i< std::min(r1.size(),r2.size()); ++i){
      error.x = std::max(error.x,std::fabs(r1[i].x - r2[i].x));
      error.y = std::max(error.y,std::fabs(r1[i].y - r2[i].y));
      error.z = std::max(error.z,std::fabs(r1[i].z - r2[i].z));
    }
    std::cout << "error: ";
    std::cout << "  {" << error.x << "," << error.y << "," << error.z << "}" << std::endl;
    double tolerance = 1e-14;
    t_assert("insert result error test", error.x < tolerance && error.y < tolerance && error.z < tolerance);
  }
  { // t = 1
    auto nurbs = NURBS<dvec3, double>(points, knots, degree);
    auto r1 = nurbs.evaluate_all(0.125);
    nurbs.knot_insert(1);
    std::vector<double> a = {0, 0, 0, 0, 1./5, 2./5,3./5, 4./5, 1 , 1, 1, 1, 1};
    t_assert("inserted knot test", nurbs.knots() == a);

    auto r2 = nurbs.evaluate_all(0.125);
    dvec3 error{0, 0, 0};
    for(size_t i=0; i< std::min(r1.size(),r2.size()); ++i){
      error.x = std::max(error.x,std::fabs(r1[i].x - r2[i].x));
      error.y = std::max(error.y,std::fabs(r1[i].y - r2[i].y));
      error.z = std::max(error.z,std::fabs(r1[i].z - r2[i].z));
    }
    std::cout << "error: ";
    std::cout << "  {" << error.x << "," << error.y << "," << error.z << "}" << std::endl;
    double tolerance = 1e-14;
    t_assert("insert result error test", error.x < tolerance && error.y < tolerance && error.z < tolerance);
  }
}
void weighted_nurbs(){
  {
    size_t degree = 3;
    std::vector<dvec4> points = {
      {-70, -76, 20, 1},
      {-70, 75, 50, 0.5},
      {74, 75, -23, 4},
      {74, -77, -1, 5},
      {-40, -76, 0, 1}
    };
    auto knots = CreateClampedKnots(points.size(), degree);
    NURBS<dvec3, double> nurbs{points, knots, degree};
    auto r1 = nurbs.evaluate_all(0.125);
    nurbs.knot_insert(0.4);
    auto r2 = nurbs.evaluate_all(0.125);
    dvec3 error{0, 0, 0};
    for(size_t i=0; i< std::min(r1.size(),r2.size()); ++i){
      error.x = std::max(error.x,std::fabs(r1[i].x - r2[i].x));
      error.y = std::max(error.y,std::fabs(r1[i].y - r2[i].y));
      error.z = std::max(error.z,std::fabs(r1[i].z - r2[i].z));
    }
    std::cout << "error: ";
    std::cout << "  {" << error.x << "," << error.y << "," << error.z << "}" << std::endl;
    double tolerance = 1e-13;
    t_assert("insert result error test", error.x < tolerance && error.y < tolerance && error.z < tolerance);
  }
}
void strange_knot(){
    size_t degree =3;
  std::vector<dvec4> points = {
    {1,1,1,1},
    {2,2,1,1},
    {3,1,1,1},
    {4,1,1,1}
  };
  { 
    std::vector<double> knots(points.size() + degree + 1);
    knots.back() = 1;
    NURBS<dvec3, double> nurbs{points, knots, degree}; 
    auto r1 = nurbs.evaluate_all(0.125);
    nurbs.knot_insert(1);
    auto r2 = nurbs.evaluate_all(0.125);
    dvec3 error{0, 0, 0};
    for(size_t i=0; i< std::min(r1.size(),r2.size()); ++i){
      error.x = std::max(error.x,std::fabs(r1[i].x - r2[i].x));
      error.y = std::max(error.y,std::fabs(r1[i].y - r2[i].y));
      error.z = std::max(error.z,std::fabs(r1[i].z - r2[i].z));
    }
    std::cout << "error: ";
    std::cout << "  {" << error.x << "," << error.y << "," << error.z << "}" << std::endl;
    double tolerance = 1e-13;
    t_assert("insert result error test", error.x < tolerance && error.y < tolerance && error.z < tolerance);
  }
  { 
    std::vector<double> knots(points.size() + degree + 1, 1.);
    knots.front() = 0;
    NURBS<dvec3, double> nurbs{points, knots, degree}; 
    auto r1 = nurbs.evaluate_all(0.125);
    nurbs.knot_insert(1);
    auto r2 = nurbs.evaluate_all(0.125);
    dvec3 error{0, 0, 0};
    for(size_t i=0; i< std::min(r1.size(),r2.size()); ++i){
      error.x = std::max(error.x,std::fabs(r1[i].x - r2[i].x));
      error.y = std::max(error.y,std::fabs(r1[i].y - r2[i].y));
      error.z = std::max(error.z,std::fabs(r1[i].z - r2[i].z));
    }
    std::cout << "error: ";
    std::cout << "  {" << error.x << "," << error.y << "," << error.z << "}" << std::endl;
    double tolerance = 1e-13;
    t_assert("insert result error test", error.x < tolerance && error.y < tolerance && error.z < tolerance);
  }
}
int main(){
  test::test_name = "knot insertion test";
  ex1();
  weighted_nurbs();
  strange_knot();
  test::summarize();
  return test::test_failed;
}