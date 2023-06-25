#pragma once

#include <sstream>
#include <stdexcept>

class YamlOptimizerError : public std::runtime_error
{
    YamlOptimizerError(std::string& what) : std::runtime_error(what) {}
};

#define DEBUG_ASSERT(condition) DEBUG_ASSERT_WITH_MSG(condition, "");

#define DEBUG_ASSERT_WITH_MSG(condition, description)                          \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            std::ostringstream oss;                                            \
            oss << "Assertion failed: " << description << " in file "          \
                << __FILE__ << " at line " << __LINE__;                        \
            throw std::runtime_error(oss.str());                               \
        }                                                                      \
    } while (false);
