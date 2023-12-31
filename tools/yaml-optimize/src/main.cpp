#include "cli.h"

#include "debug/exception.h"
#include "optimizer/optimizer.h"

#include <fmt/format.h>

#include <fstream>
#include <iostream>

#ifdef YO_DEBUG
#pragma message("Building debug version of optimizer")
#else
#pragma message("Building release version of optimizer")
#endif

int main(int argc, char* argv[])
{
    try
    {
        cli_utils::Args args{};
        if (!cli_utils::parse(argc, argv, args))
            return 0;

        std::ifstream is(args.in_fname);

        YamlOptimizer optimizer(is, args.settings);

        optimizer.optimize();

        optimizer.dump(args.out_fname);
    }
    catch (const cli_utils::ArgParseError& e)
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
