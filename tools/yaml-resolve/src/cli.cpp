#include "cli.h"

#include <filesystem>
#include <iostream>

#include <lyra/lyra.hpp>

namespace cli_utils
{

ArgParseError::ArgParseError(std::string const& what) : std::runtime_error(what)
{
}

bool parse(int argc, char* argv[], Args& args)
{
    int optimization_limit = -1;
    auto cli =
        lyra::help(args.show_help)
            .description("YAML Resolver - A tool for resolving YAML optimized "
                         "configurations with anchors, references and merge "
                         "keys, resolving them into full YAML configuration") |
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
            "path to output file (defaults to input filename + \".resolved\")")
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

    // Set output filename to input filename with ".optimized" appended
    // if output isn't specified
    if (args.out_fname.empty())
        args.out_fname = args.in_fname + ".resolved";

    if (args.verbose)
    {
        std::cout << "Input file: " << args.in_fname << std::endl;
        std::cout << "Output file: " << args.out_fname << std::endl;
    }

    return true;
}

} // namespace cli_utils
