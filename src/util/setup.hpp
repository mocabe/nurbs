#pragma once

#if defined(NDEBUG)
constexpr bool is_debug = false;
#else
#include <iostream>
constexpr bool is_debug = true;
#endif

#include "dimension.hpp"
#include "access.hpp"
#include "vector_traits.hpp"
// vector algo
#include "algorithm.hpp"