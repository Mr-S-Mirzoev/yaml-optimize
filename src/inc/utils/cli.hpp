#pragma once

#include "../optimizer/settings.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <lyra/lyra.hpp>

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
    ArgParseError(std::string const& what) : std::runtime_error(what) {}
};

bool parse(int argc, char* argv[], Args& args)
{
    int optimization_limit = -1;
    auto cli =
        lyra::help(args.show_help)
            .description(
                "YAML Optimizer - A tool for optimizing YAML configurations "
                "with the use of anchors, references and merge keys") |
        lyra::opt(optimization_limit, "optimisation limit")["-l"]["--limit"](
            "limit to when to stop optimisation")
            .optional()
            .choices(
                [](int optimization_limit)
                {
                    if (optimization_limit >= 0)
                        return true;

                    std::cerr
                        << "Optimization Limit must be a non-negative integer."
                        << std::endl;
                    return false;
                }) |
        lyra::opt(args.in_fname, "input")["-i"]["--input"]("path to input file")
            .required()
            .choices(
                [](std::string input_filename)
                {
                    if (std::filesystem::exists(input_filename))
                        return true;

                    std::cerr << "Input file doesn't exist: " << input_filename
                              << std::endl;
                    return false;
                }) |
        lyra::opt(args.out_fname, "output")["-o"]["--output"](
            "path to output file (defaults to input filename + \".optimized\")")
            .optional() |
        lyra::opt(args.verbose)["-v"]["--verbose"]("enable verbose mode")
            .optional();

    auto result = cli.parse({argc, argv});
    if (!result)
    {
        std::cerr << "Error in command line: " << result.message() << std::endl;
        return false;
    }

    if (args.show_help)
    {
        std::cout << cli << std::endl;
        return false;
    }

    if (optimization_limit != -1)
        args.settings.optimization_limit =
            static_cast<std::size_t>(optimization_limit);

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

} // namespace cli_utils
