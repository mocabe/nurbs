#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include <nurbs.hpp>
#include <test/test.hpp>
#include <test/2D/config/adaptor.hpp>
using namespace nurbs;

int main(){

  nurbs::test::summarize();
  return nurbs::test::test_failed;
}