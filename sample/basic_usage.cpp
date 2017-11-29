
#include <nurbs.hpp>
#include <sample/simple_vector_adaptor.hpp>

using namespace smplv;
using namespace nurbs;
using namespace nurbs::tags;
int main(){
  // 次数
  size_t degree = 3;

  // 制御点
  std::vector<dvec4> points = {
      {2, 5, 0, 1},  {3, 10, 0, 1},  {8, 13, 0, 1}, {10, 4, 0, 0},
      {20, 7, 0, 0}, {19, 20,0 , 1}, {1, 21, 0, 1}};

  // ノットベクトル
  auto knots =  nurbs::CreateClampedKnots<double>(points.size(),degree);

  // NURBSクラスの初期化
  nurbs::NURBS<dvec4, double> nurbs{points, knots, degree};

  // 曲線を100サンプル程度計算する
  auto result = nurbs.evaluate_all<NonRecursive<>>(0.01);

  // 結果を使って何かする
}