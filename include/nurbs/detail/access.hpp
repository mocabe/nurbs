//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// Distributed under LGPLv3 License. See LICENSE for more details.
//

#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace nurbs {

  template <class PointT, size_t Degree>
  struct access;

  template <size_t Degree, class PointT>
  constexpr decltype(auto) get(PointT &&t)
  {
    return access<std::decay_t<PointT>, Degree>::get(std::forward<PointT>(t));
  }

}