#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <type_traits>
namespace nurbs{
  namespace detail {
  template <int FBegin, int TBegin, int Range, class From, class To>
  struct Map_impl {
    static constexpr void map(const From &from, To &to) {
      get<FBegin + Range - 1>(to) = get<TBegin + Range - 1>(from);
      Map_impl<FBegin, TBegin, Range - 1, From, To>::map(from, to);
    }
  };

  template <int FBegin, int TBegin, class From, class To>
  struct Map_impl<FBegin, TBegin, 0, From, To> {
    static constexpr void map(const From &, To &) {}
  };

  template <int FBegin, int TBegin, int Range, class From, class To>
  constexpr void map(const From &from, To &to) {
    Map_impl<FBegin, TBegin, Range, From, To>::map(from, to);
  }
  } // namespace nurbs::detail

  // cast
  template <class To, class From> 
  constexpr To degenerate(const From &from) {
    To to{};
    detail::map<0, 0, dimension_v<To>, From, To>(from, to);
    return to;
  }
}//namespace nurbs