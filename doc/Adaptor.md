## ベクトルクラスを対応する

以下の構造体を設定してみましょう
```cpp
  struct dvec2 {
    double x, y;
  };
  struct dvec3 {
    double x,y,z;
  };
```
特殊化のコード全体は[`sample/simple_vector_adaptor.hpp`](../sample/simple_vector_adaptor.hpp)あたりを見てください  

#### `nurbs::access`の特殊化
利用するベクトルクラスのアクセサです。  
ライブラリはN次元ベクトルの重みはN+1次元目の要素としてアクセスします。

``` cpp
template <> struct access<dvec3, 0> {
  static constexpr auto &get(dvec3 &v) { return v[0]; }
  static constexpr const auto &get(const dvec3 &v) { return v[0]; }
};
```
このようにして全部の次元にアクセサを作ります  

#### `nurbs::dimension`の特殊化
nurbs::dimensionで制御点の次元を指定してください

``` cpp
template <> struct dimension<dvec2> { static constexpr size_t value = 2; };
template <> struct dimension<dvec3> { static constexpr size_t value = 3; };
```

#### `nurbs::point_traits`の特殊化
point_traitsでは、ライブラリが使用するベクトルクラスを設定します。
`point_type`と`weighted_point`を設定します

``` cpp
template <> struct point_traits<dvec2> {
  using point_type = dvec2;
  using weighted_point_type = dvec3;
};
```