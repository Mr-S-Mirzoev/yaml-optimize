#pragma once

#include "debug/exception.hpp"
#include "debug/throw.hpp"

#include <sstream>

#define YO_DEBUG_ASSERT(condition) YO_DEBUG_ASSERT_WITH_MSG(condition, "");

#define YO_DEBUG_ASSERT_WITH_MSG(condition, description, ...)                          \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            YO_THROW(YamlOptimizerError, "Assertion failed: {} in file " __FILE__ " at line {}", fmt::format(description, ##__VA_ARGS__), __LINE__) \
        }                                                                      \
    } while (false);
