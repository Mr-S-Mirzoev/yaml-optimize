#pragma once

#include "../debug/throw.hpp"
#include "../optimizer/settings.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#define CLARA_CONFIG_OPTIONAL_TYPE std::optional
#include "clara.hpp"

namespace cli
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
    ArgParseError(std::string const& what) : std::runtime_error(what) {}
};

bool parse(int argc, char* argv[], Args& args)
{
    auto cli =
        clara::Help(args.show_help) |
        clara::Opt(args.settings.optimization_limit, "optimization limit")["-l"]["--limit"](
            "limit to when to stop optimization") |
        clara::Opt(args.in_fname,
                   "input")["-i"]["--input"]("path to input file") |
        clara::Opt(args.out_fname, "output")["-o"]["--output"](
            "path to output file (optional; if not specified, output is "
            "written to `input.ext.optimized`)") |
        clara::Opt(args.verbose)["-v"]["--verbose"]("enable verbose mode");

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result)
        YO_THROW(ArgParseError, "Error in command line: {}", result.errorMessage());

    if (args.show_help)
    {
        cli.writeToStream(std::cout);
        return false;
    }

    if (args.in_fname.empty())
        YO_THROW(ArgParseError, "Input filename {} not specified.", args.in_fname);

    if (!std::filesystem::exists(args.in_fname))
        YO_THROW(ArgParseError, "Input file {} doesn't exist.", args.in_fname);

    // Set output filename to input filename with ".optimized" appended
    // if output isn't specified
    if (args.out_fname.empty())
        args.out_fname = args.in_fname + ".optimized";

    if (args.verbose)
    {
        std::cout << "Input file: " << args.in_fname << std::endl;
        std::cout << "Output file: " << args.out_fname << std::endl;
    }

    return true;
}

} // namespace cli
