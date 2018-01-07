[![Build Status](https://travis-ci.org/mocabe/nurbs.svg?branch=master)](https://travis-ci.org/mocabe/nurbs) [![Build status](https://ci.appveyor.com/api/projects/status/r5slhpbsgb7mny1r/branch/master?svg=true)](https://ci.appveyor.com/project/mocabe/nurbs)
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
  nurbs::NURBS<dvec4, double> nurbs{points, knots, degree};

  // 曲線を100サンプル程度計算する
  auto result = nurbs.evaluate_all(0.01);

  // 結果を使って何かする
```

## Usage  

### ベクトルクラスの設定
[こちら](doc/Adaptor.md)を見てください

### `NURBS<T,K>`
  
Tが重み付きベクトルクラス、Kがノット列に使用する浮動小数点型です.  

NURBSの計算には幾つか関数がありますが、  
`evaluate`, `evaluate_all`は0.0~1.0の相対値を使って計算します  

`evaluate`は単一点用、まとめて計算する場合は`evaluate_all`を使います  
範囲を指定したい場合は`evaluate`に範囲を渡せます.  

- `std::vector<wpoint_type> evaluate_all(knot_type)`  
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

- `std::pair<knot_type, knot_type> knot_range() const`  
  有効なノット範囲 `{knots[degree],knots[points.size()]}` を返します  

- `NURBS<T,K>& knot_insert(knot_type)`  
  ノットを挿入します  
  ノットベクトルと制御点の数が１ずつ増加します  
  挿入するノットは`knot_range()`の範囲にクランプされます  
  挿入後の曲線はほとんど変化しません

- `NURBS<T,K>& reverse(NURBS<T,K>&)`  
  曲線の計算方向を反転させます  
  `evaluate_all(-i)`と反転後の`evaluate_all(i)`は厳密には一致しません  

- `point_type tangent(knot_type, point_type* =nullptr) const`  
  正規化されていない接ベクトルを計算します。  
  `point_type*`の引数に渡すと、接点を同時に計算します  
  (`evaluate()`で再計算するより効率が良いです)

- `std::pair<NURBS,NURBS> split(knot_type t) const`  
  曲線をt∈[0,1]で分割します。分割後の曲線は元の曲線と同じ次数になります。

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