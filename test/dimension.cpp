//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#include <mocabe/nurbs/detail/dimension.hpp>
#include <catch2/catch.hpp>
#include "vec2.hpp"

using namespace mocabe::nurbs;

TEST_CASE("dimension")
{
  STATIC_REQUIRE(dimension_v<vec2> == 2);
}