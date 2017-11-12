[![Build Status](https://travis-ci.org/mocabe/nurbs.svg?branch=master)](https://travis-ci.org/mocabe/nurbs) [![Build status](https://ci.appveyor.com/api/projects/status/r5slhpbsgb7mny1r?svg=true)](https://ci.appveyor.com/project/mocabe/nurbs)
# nurbs
ヘッダーオンリーのNURBSライブラリです。  
C++で実装されています。  
templateを使用して自前のベクトルライブラリに対応できます  

計算アルゴリズムは、De Boor's Algorithmをを実装しています  

## Compiler requirements
Requires c++14 features.  
[compiler support list](http://en.cppreference.com/w/cpp/compiler_support)

## Code example
```cpp
  // 次数
  size_t degree = 3;

  // 制御点
  std::vector<dvec4> points = {
      {2, 5, 0, 1},  {3, 10, 0, 1},  {8, 13, 0, 1}, {10, 4, 0, 0},
      {20, 7, 0, 0}, {19, 20,0 , 1}, {1, 21, 0, 1}};

  // ノットベクトル
  auto knots =  nurbs::CreateClampedKnots<double>(points.size(),degree);

  // NURBSクラスの初期化
  nurbs::NURBS<dvec3, double> nurbs{points, knots, degree};

  // 曲線を100サンプル程度計算する
  auto result = nurbs.evaluate_all(0.01);

  // 結果を使って何かする
```

## Usage  
OpenGL Mathematics(GLM) を使用した例を示します。  
特殊化のコード全体はsample/glm_adaptor.hppに入ってます  

#### `nurbs::access`の特殊化
利用するベクトルクラスのアクセサです。
ライブラリはN次元ベクトルの重みはN+1次元目の要素としてアクセスします。

``` cpp
template <> struct access<dvec3, 0> {
  static double &get(dvec3 &v) { return v[0]; }
  static const double &get(const dvec3 &v) { return v[0]; }
};
```

#### `nurbs::dimension`の特殊化
nurbs::dimensionを指定して、制御点の次元を指定してください

``` cpp
template <> struct dimension<dvec3> { static const int value = 3; };
```

#### `nurbs::point_traits`の特殊化
point_traitsでは、ライブラリが使用するベクトルクラスを設定します。
`point_type`と`weighted_point`を設定します

``` cpp
template <> struct point_traits<dvec3> {
  using point_type = dvec3;
  using weighted_point_type = dvec4;
};
```

### その他
演算子はなくてもアクセサから勝手に生成します。  


### NURBSの使用
以上の設定が終わったら、`NURBS<T,K>`が使用できます.    
Tがn次元ベクトルクラス、Kがノット列に使用する浮動小数点数です.  

NURBSの計算には幾つか関数がありますが、  
`evaluate`, `evaluate_all`は0.0~1.0の相対値  
knot_evaluateは実際のノットベクトルの値を使って計算します.  

基本的に`evaluate`か`evaluate_all`で大丈夫だと思います.  


#### `NURBS::evaluate_all(knot_type t)`
0.0~1.0の値を指定すれば、それをインターバルとして点列を計算してくれます  
例えば、0.1を設定すれば、始点と終点を含む12個の計算結果が帰ってきます  
負の値を設定すれば逆向きになります  
結果は重複する可能性があることに注意してください  


## アルゴリズム
De Boor's Algorithmを２通り実装してます。  
デフォルトでは再帰関数を使った実装を使うようになっています。  

非再帰版は```NURBS_NON_RECURSIVE```をdefineすれば切り替わります.  
非再帰版はデフォルトでヒープ領域に計算用配列を用意しますが、  
スタックに確保したい場合は更に```NURBS_ENABLE_STACK_ALLOCATION```をdefineしてください。  
この場合は非標準の関数```alloca()```を使います  

このライブラリは基底関数が全てゼロのような計算不能な場合でも一番妥当な位置を返そうとします  
{0,0,0}とか{nan,nan,nan}みたいなのが返ってくるのはできるだけ避けているつもりです

## ユーティリティ
`CreateUniformKnots`関数で均一なノットベクトルを生成できます.  
`CreateClampedKnots`はクランプされたノットベクトルを生成します.  

とりあえず`CreateClampedKnots`で作れば始点と終点を通るようになります  

## マルチスレッド
```NURBS_ENABLE_MULTI_THREADING```を有効にすれば簡易マルチスレッディングができます  
スレッド数は```NURBS_THREAD_NUM```で設定できます(デフォルトは8)
あくまで簡易なのであまり良くスケールしません  

## TODO
コードのクリーンアップとバグ修正  
テストの追加  

## Future
適応分割計算  
その他ユーティリティとアルゴリズムの追加  
曲面対応  

## Reference
参考にした記事やサイトなど  
[wikipedia :のっている式はインデックスが1から始まるので注意](https://ja.wikipedia.org/wiki/NURBS)  
[ミシガン工科大学のCS3621講義ノート :リンクはDe Boor's Algorithmの解説です。](https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/de-Boor.html)  
[fussyさんの記事 :これ以外にも色々アルゴリズムの解説があります](http://fussy.web.fc2.com/algo/curve3_b-spline.htm)  
## Licence
MIT Licence

