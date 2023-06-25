#include "optimizer/optimizer.hpp"

#include <filesystem>
#include <iostream>

#include "clara.hpp"

int main(int argc, char* argv[])
{
    std::string app_name;
    std::string in_fname;
    std::string out_fname;
    bool verbose = false;
    bool show_help = false;

    auto cli =
        clara::Help(show_help) |
        clara::Opt(in_fname, "input")["-i"]["--input"]("path to input file") |
        clara::Opt(out_fname, "output")["-o"]["--output"](
            "path to output file (optional; if not specified, output is "
            "written to `input.ext.optimized`)") |
        clara::Opt(verbose)["-v"]["--verbose"]("enable verbose mode");

    auto result = cli.parse(clara::Args(argc, argv));
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage()
                  << std::endl;
        return 1;
    }

    if (show_help)
    {
        cli.writeToStream(std::cout);
        return 0;
    }

    if (in_fname.empty())
    {
        std::cerr << "Input filename not specified." << std::endl;
        return 1;
    }

    if (!std::filesystem::exists(in_fname))
    {
        std::cerr << "Input filename doesn't exist." << std::endl;
        return 1;
    }

    // Set output filename to input filename with ".optimized" appended
    // if output isn't specified
    if (out_fname.empty())
        out_fname = in_fname + ".optimized";

    if (verbose)
    {
        std::cout << "Input file: " << in_fname << std::endl;
        std::cout << "Output file: " << out_fname << std::endl;
    }

    std::ifstream is(in_fname);

    YamlOptimizer optimizer(is);

    optimizer.optimize();

    return 0;
}
