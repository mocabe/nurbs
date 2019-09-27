//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#include <mocabe/nurbs/detail/access.hpp>
#include <catch2/catch.hpp>
#include "vec2.hpp"

using namespace mocabe::nurbs;

TEST_CASE("access")
{
  vec2 vec;

  vec.x = 42;
  vec.y = 3.14;

  REQUIRE(get<0>(vec) == 42);
  REQUIRE(get<1>(vec) == 3.14);

  get<0>(vec) = 24;
  REQUIRE(get<0>(vec) == 24);
}