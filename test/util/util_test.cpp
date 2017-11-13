#include <src/util/nurbs_util.hpp>
#include <test/test.hpp>

using namespace nurbs;

void uniform(){
  {
    auto knots = CreateUniformKnots(6,2);
    std::vector<double> a = {0, 1./8, 2./8, 3./8, 4./8, 5./8, 6./8, 7./8, 1};
    t_assert("uniform", knots == a);
  }
  {
    try{
      auto knots = CreateUniformKnots(0, 0);
      t_assert("uniform numPoint==0", false);
    }catch(std::invalid_argument& e){
      ++test::test_run;
    }
  }
  {
    try { 
      auto knots = CreateUniformKnots(1, 1);
      t_assert("uniform degree > numPoints- 1", false);
    }catch(std::invalid_argument& e){
      ++test::test_run;
    }
  }
  {
    auto knots = CreateUniformKnots(1,0);
    std::vector<double> a = {0,1};
    t_assert("uniform numPoints:1, degree:0", knots==a);
  }
}

void clamped(){
  {
    auto knots = CreateClampedKnots(6,2);
    std::vector<double> a = {0, 0, 0, 1./4, 2./4, 3./4, 1, 1, 1};
    t_assert("clamped", knots == a);
  }
  {
    try{
      auto knots = CreateClampedKnots(0, 0);
      t_assert("clamped numPoint==0", false);
    }catch(std::invalid_argument& e){
      ++test::test_run;
    }
  }
  {
    try { 
      auto knots = CreateClampedKnots(1, 1);
      t_assert("clamped degree > numPoints- 1", false);
    }catch(std::invalid_argument& e){
      ++test::test_run;
    }
  }
  {
    auto knots = CreateClampedKnots(1,0);
    std::vector<double> a = {0,1};
    t_assert("clamped numPoints:1, degree:0", knots==a);
  }
}

int main(){
    uniform();
    clamped();
    test::summarize();
    return test::test_failed;
}