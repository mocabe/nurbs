//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#include <mocabe/nurbs/detail/tvec.hpp>
#include <catch2/catch.hpp>

using namespace mocabe::nurbs;

TEST_CASE("tvec2")
{
  tvec2<int> vec;

  REQUIRE(vec[0] == 0);
  REQUIRE(vec[1] == 0);
  REQUIRE(get<0>(vec) == 0);
  REQUIRE(get<1>(vec) == 0);

  vec += 2;

  REQUIRE(vec[0] == 2);
  REQUIRE(vec[1] == 2);

  get<0>(vec) = 1;
  get<1>(vec) = 1;

  vec *= 42;

  REQUIRE(vec[0] == 42);
  REQUIRE(vec[1] == 42);

  vec -= 24;

  REQUIRE(vec[0] == 18);
  REQUIRE(vec[1] == 18);

  vec /= 9;

  REQUIRE(vec[0] == 2);
  REQUIRE(vec[1] == 2);

  tvec2<int> vec2 {21};

  REQUIRE((vec * vec2 / 42)[0] == 1);
  REQUIRE((vec * vec2 / 42)[1] == 1);
}