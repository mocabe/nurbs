#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include <nurbs.hpp>
#include <test/test.hpp>
#include <test/2D/config/adaptor.hpp>
using namespace nurbs;

int main(){
  test::test_name  = "1 degree tests";
  nurbs::test::summarize();
  return nurbs::test::test_failed;
}