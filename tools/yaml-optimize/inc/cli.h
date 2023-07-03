#pragma once

#include "optimizer/settings.h"

#include <stdexcept>
#include <string>

namespace cli_utils
{

struct Args
{
    OptimizationSettings settings;
    std::string app_name;
    std::string in_fname;
    std::string out_fname;
    bool verbose = false;
    bool show_help = false;
};

class ArgParseError : public std::runtime_error
{
public:
    ArgParseError(std::string const& what);
};

bool parse(int argc, char* argv[], Args& args);

} // namespace cli_utils
