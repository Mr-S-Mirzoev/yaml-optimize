#pragma once

#include <sstream>
#include <stdexcept>

class YamlOptimizerError : public std::runtime_error
{
public:
    YamlOptimizerError(std::string const& what) : std::runtime_error(what) {}
};

#define DEBUG_ASSERT(condition) DEBUG_ASSERT_WITH_MSG(condition, "");

#define DEBUG_ASSERT_WITH_MSG(condition, description)                          \
    do                                                                         \
    {                                                                          \
        if (!(condition))                                                      \
        {                                                                      \
            std::ostringstream oss;                                            \
            oss << "Assertion failed: " description " in file "  __FILE__ " at line " << __LINE__;                        \
            throw YamlOptimizerError(oss.str());                               \
        }                                                                      \
    } while (false);
