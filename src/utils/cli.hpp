#pragma once

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include "clara.hpp"
#include "fmt/format.h"

namespace cli
{
struct Args
{
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
        clara::Opt(args.in_fname,
                   "input")["-i"]["--input"]("path to input file") |
        clara::Opt(args.out_fname, "output")["-o"]["--output"](
            "path to output file (optional; if not specified, output is "
            "written to `input.ext.optimized`)") |
        clara::Opt(args.verbose)["-v"]["--verbose"]("enable verbose mode");

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result)
        throw ArgParseError(
            fmt::format("Error in command line: {}", result.errorMessage()));

    if (args.show_help)
    {
        cli.writeToStream(std::cout);
        return false;
    }

    if (args.in_fname.empty())
        throw ArgParseError(
            fmt::format("Input filename {} not specified.", args.in_fname));

    if (!std::filesystem::exists(args.in_fname))
        throw ArgParseError(
            fmt::format("Input file {} doesn't exist.", args.in_fname));

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