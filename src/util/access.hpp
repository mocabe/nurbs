#pragma once


#include <type_traits>
namespace nurbs{

template <class T, int I>
struct access;

template <int I, class T> 
constexpr decltype(auto) get(T &&t) {
  return access<typename std::decay<T>::type, I>::get(std::forward<T>(t));
}
}
