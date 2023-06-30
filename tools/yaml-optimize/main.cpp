#include "debug/exception.hpp"
#include "optimizer/optimizer.h"
#include "utils/cli.hpp"

#include <fstream>

#ifdef YO_DEBUG
#pragma message("Building debug version of optimizer")
#else
#pragma message("Building release version of optimizer")
#endif

int main(int argc, char* argv[])
{
    try
    {
        cli::Args args{};
        if (!cli::parse(argc, argv, args))
            return 0;

        std::ifstream is(args.in_fname);

        YamlOptimizer optimizer(is, args.settings);

        optimizer.optimize();

        optimizer.dump(args.out_fname);
    }
    catch (const cli::ArgParseError& e)
    {
        std::cerr << fmt::format("Error during CLI args parsing occured: {}",
                                 e.what())
                  << std::endl;
        return 1;
    }
    catch (const YamlOptimizerError& e)
    {
        std::cerr << fmt::format("Error during optimization occured: {}",
                                 e.what())
                  << std::endl;
        return 1;
    }

    return 0;
}
