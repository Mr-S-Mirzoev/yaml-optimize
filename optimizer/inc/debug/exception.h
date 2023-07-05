#pragma once

#include <stdexcept>

class YamlOptimizerError : public std::runtime_error
{
public:
    YamlOptimizerError(std::string const& what) : std::runtime_error(what) {}
};
