#pragma once

#if defined(NDEBUG)
constexpr bool is_debug = false;
constexpr unsigned debug_msg_level = 0;
#else
#include <iostream>
constexpr bool is_debug = true;
#if defined(NURBS_DEBUG_MSG_LEVEL)
constexpr unsigned debug_msg_level = NURBS_DEBUG_MSG_LEVEL;
#else
constexpr unsigned debug_msg_level = 1;
#endif
#endif

#include "dimension.hpp"
#include "access.hpp"
#include "vector_traits.hpp"
// vector algo
#include "algorithm.hpp"