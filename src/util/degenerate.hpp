#pragma once
#include "dimension.hpp"
#include "access.hpp"
#include <type_traits>
namespace nurbs{
  namespace detail {
  template <int FBegin, int TBegin, int Range, class From, class To> struct Map_impl {
    static void map(const From &from, To &to) {
      get<FBegin + Range - 1>(to) = get<TBegin + Range - 1>(from);
      Map_impl<FBegin, TBegin, Range - 1, From, To>::map(from, to);
    }
  };

  template <int FBegin, int TBegin, class From, class To>
  struct Map_impl<FBegin, TBegin, 0, From, To> {
    static void map(const From &, To &) {}
  };

  template <int FBegin, int TBegin, int Range, class From, class To> struct Map {
    static void map(const From &from, To &to) {
      Map_impl<FBegin, TBegin, Range, From, To>::map(from, to);
    }
  };
  } // namespace nurbs::detail

  // cast
  template <class To, class From> To degenerate(const From &from) {
    To to;
    detail::Map<0, 0, dimension_v<To>, From, To>::map(from, to);
    return to;
  }
}//namespace nurbs