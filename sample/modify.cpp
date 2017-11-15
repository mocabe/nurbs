#include <nurbs.hpp>
#include <sample/simple_vector_adaptor.hpp>
#include <iostream>
#include <cassert>
using namespace nurbs;

// Example of modifying knots and points in NURBS class.
int main(){

  /* 
    You can read/modify control points from
     1. index based access: pget()
     2. iterator: pbegin() / pend()
    * points() returns copy of current points.
    * set_points() can replace current points with new ones.

    You can READ knot vector from
     1. index based access: kget()
     2. iterator: kbegin() / kend()
     3. creating copy: knots()
     * set_knots() is the only way to modify knots.
  */

  size_t degree = 3;
  std::vector<dvec4> points = {
      {1, 0, 0, 1}, {2, 4, 0, 1}, {3, 4, 0, 1}, {4, 0, 0, 1}};
  auto knots = CreateClampedKnots(points.size(), degree);

  /// NURBS
  NURBS<dvec3, double> nurbs{points, knots, degree};

  // copying points and knots from NURBS class.
  auto points_copy = nurbs.points();
  auto knots_copy = nurbs.knots();

  // pget() returns const/non-const reference.
  auto p2 = nurbs.pget(2);
  nurbs.pget(1) = {4, 2, 0, 1}; 

  // kget() return const reference.
  auto k0 = nurbs.kget(0); 

  // pbegin()/pend() returns iterator/const_iterator
  auto points_itr = nurbs.pbegin();
  points_itr->z = 42;

  // kbegin()/kend() returns const_iterator
  auto knots_itr = nurbs.kbegin();

  // setting new knots.
  auto new_knots = CreateUniformKnots(points.size(), degree);
  assert(nurbs.set_knots(new_knots));
}
