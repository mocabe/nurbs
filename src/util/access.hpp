#pragma once


#include <type_traits>
namespace nurbs{

template <class T, size_t I>
struct access;

template <size_t I, class T> 
constexpr decltype(auto) get(T &&t) {
  return access<typename std::decay<T>::type, I>::get(std::forward<T>(t));
}

template <size_t I, class T>
constexpr decltype(auto) set( T t, std::remove_reference_t<decltype(get<I>(t))> v ){
  get<I>(t) = v;
  return t;
}
}
