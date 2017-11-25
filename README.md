[![Build Status](https://travis-ci.org/mocabe/nurbs.svg?branch=master)](https://travis-ci.org/mocabe/nurbs) [![Build status](https://ci.appveyor.com/api/projects/status/r5slhpbsgb7mny1r?svg=true)](https://ci.appveyor.com/project/mocabe/nurbs)
# nurbs
ヘッダーオンリーのNURBSライブラリです。  
C++で実装されています。  
templateを使用して自前のベクトルライブラリに対応できます  

計算アルゴリズムは、De Boor's Algorithmを実装しています  

## Compiler requirements
Requires c++17 features.  
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
特殊化のコード全体は`sample/glm_adaptor.hpp`に入ってます  

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
template <> struct dimension<dvec3> { static constexpr size_t value = 3; };
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


- `std::vector<wpoint_type> NURBS<T,K>::evaluate_all(knot_type)`  
0.0~1.0の値を指定すれば、それをインターバルとして点列を計算してくれます  
例えば、0.1を設定すれば、始点と終点を含む12個の計算結果が帰ってきます  
負の値を設定すれば逆向きになります  
結果は重複する可能性があることに注意してください  


## アルゴリズム
De Boor's Algorithmを２通り実装してます。  
デフォルトでは再帰関数を使った実装を使うようになっています。  

`nurbs::tags` 名前空間に入っているタグを使って切り替えられます
```cpp
  // 非再帰関数版(スタック上にバッファを確保)
  nurbs.evaluate_all<NonRecursive<StackArray>>(0.1); 
```

デフォルトでは`tags::Recursive`が設定さ設定されています.  
非再帰版をデフォルトに設定する場合は```NURBS_NON_RECURSIVE```をdefineしてください.  
非再帰版はデフォルトでヒープ領域に計算用配列を用意します  
スタックに確保したい場合は更に```NURBS_ENABLE_STACK_ALLOCATION```をdefineしてください。  
この場合は非標準の関数```alloca()```を使います  

このライブラリは基底関数が全てゼロのような計算不能な場合でも一番妥当な位置を返そうとします  
{0,0,0}とか{nan,nan,nan}みたいなのが返ってくるのはできるだけ避けているつもりです

## ユーティリティ  
- `CreateUniformKnots`  
  関数で均一なノットベクトルを生成できます.  

- `CreateClampedKnots`  
  クランプされたノットベクトルを生成します.  

とりあえず`CreateClampedKnots`で作れば始点と終点を通るようになります  

## その他関数

### メンバ関数  

- `std::pair<knot_type, knot_type> NURBS<T,K>::knot_range() const`  
  有効なノット範囲 `{knots[degree],knots[points.size()]}` を返します  

- `NURBS<T,K>& NURBS<T,K>::knot_insert(knot_type)`  
  ノットを挿入します  
  ノットベクトルと制御点の数が１ずつ増加します  
  挿入するノットは`knot_range()`の範囲にクランプされます  
  挿入後の曲線はほとんど変化しません

### 非メンバ関数  

- `NURBS<T,K>& nurbs::reverse(NURBS<T,K>&)`  
  曲線の計算方向を反転させます  
  `evaluate_all(-i)`と反転後の`evaluate_all(i)`は厳密には一致しません  

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