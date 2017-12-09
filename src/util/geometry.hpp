#pragma once

#include <cmath>
namespace nurbs {

namespace impl{
template <class V, size_t N> 
struct dot_impl {
  static constexpr auto dot(const V &lhs, const V &rhs) {
    return std::fma(get<N - 1>(lhs), get<N - 1>(rhs),
                    dot_impl<V, N - 1>::dot(lhs, rhs));
  }
};

template <class V> 
struct dot_impl<V, 1> {
  static constexpr auto dot(const V &lhs, const V &rhs) {
    return get<0>(lhs) * get<0>(rhs);
  }
};

template <class V> 
struct dot_impl<V, 0> {};
}

template <class V>
constexpr auto dot(const V& lhs, const V& rhs){
  return impl::dot_impl<V, dimension_v<V>>::dot(lhs, rhs);
}


template <class V, class = std::enable_if_t<dimension_v<V> == 3>>
constexpr V cross(const V& lhs, const V& rhs) {
  V v {};
  get<0>(v) =  get<1>(lhs) * get<2>(rhs) - get<2>(lhs) * get<1>(rhs);
  get<1>(v) =  get<2>(lhs) * get<0>(rhs) - get<0>(lhs) * get<2>(rhs);
  get<2>(v) =  get<0>(lhs) * get<1>(rhs) - get<1>(lhs) * get<0>(rhs);
  return v;
}


template <class V>
constexpr auto distance(const V& lhs, const V& rhs) {
  auto v = rhs - lhs;
  return std::sqrt(dot(v, v));
};

template <class V>
constexpr V normalize(const V& v) {
  auto div = std::sqrt(dot(v, v));
  if (div == 0)
    return v;
  return v / div;
}

}