//
// Copyright (c) 2019 mocabe (https://github.com/mocabe)
// SPDX-License-Identifier: MIT
//

#include <mocabe/nurbs/detail/element_type.hpp>
#include <catch2/catch.hpp>
#include "vec2.hpp"

using namespace mocabe::nurbs;

TEST_CASE("element_type")
{
  STATIC_REQUIRE(std::is_same_v<element_type_t<vec2>, float>);
}

TEST_CASE("common_type")
{
  STATIC_REQUIRE(std::is_same_v<compute_type_t<vec2, float>, float>);
  STATIC_REQUIRE(std::is_same_v<compute_type_t<vec2, double>, double>);
}