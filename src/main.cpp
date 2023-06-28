#include "optimizer/optimizer.hpp"
#include "utils/cli.hpp"

int main(int argc, char* argv[])
{
    try
    {
        cli::Args args{};
        if (!cli::parse(argc, argv, args))
            return 0;

        std::ifstream is(args.in_fname);

        YamlOptimizer optimizer(is);

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
