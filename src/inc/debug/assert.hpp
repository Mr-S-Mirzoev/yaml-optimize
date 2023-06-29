#pragma once

#include "debug/exception.hpp"

#include <sstream>

#define DEBUG_ASSERT(condition) DEBUG_ASSERT_WITH_MSG(condition, "");

#define DEBUG_ASSERT_WITH_MSG(condition, description)                          \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            std::ostringstream oss;                                            \
            oss << "Assertion failed: " description                            \
                   " in file "  __FILE__ " at line " << __LINE__;              \
            throw YamlOptimizerError(oss.str());                               \
        }                                                                      \
    } while (false);
