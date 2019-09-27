//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#include <mocabe/nurbs/detail/access.hpp>

struct vec2
{
  float x, y;
};

template <>
struct mocabe::nurbs::vector_access<vec2, 0>
{
  static float& get(vec2& v)
  {
    return v.x;
  }

  static const float& get(const vec2& v)
  {
    return v.x;
  }
};

template <>
struct mocabe::nurbs::vector_access<vec2, 1>
{
  static float& get(vec2& v)
  {
    return v.y;
  }

  static const float& get(const vec2& v)
  {
    return v.y;
  }
};