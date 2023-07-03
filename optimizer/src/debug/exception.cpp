#include "debug/exception.h"

YamlOptimizerError::YamlOptimizerError(std::string const& what)
    : std::runtime_error(what)
{
}
